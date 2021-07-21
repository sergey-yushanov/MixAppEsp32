#include <Collector.h>

void Collector::closeAll()
{
    for (int i = 0; i < nValves_; i++)
        valves[i].close();
    valveAdjustable.fullyClose();
}

// void Collector::openToFill()
// {
//     valves[nValves_ - 1].open();
//     if (valves[nValves_ - 1].isCommand())
//         valveAdjustable.fullyOpen();
// }

// void Collector::closeAfterFilling()
// {
//     valveAdjustable.fullyClose();
//     if (valveAdjustable.isClosed())
//         valves[nValves_ - 1].close();
// }

// void Collector::closeAfterDosing(int valveNum)
// {
//     valveAdjustable.fullyClose();
//     if (valveAdjustable.isClosed())
//         valves[valveNum].close();
// }

// наполнение коллектора носителем
void Collector::fill()
{
    // стартуем с заполнением коллектора
    if (fillingStart_ && !filling_ && !fillingFinishing_ && !fillingDone_)
    {
        flowmeter.nullifyVolume();
        filling_ = true;
    }
    fillingStart_ = false;

    // открываем клапаны для наполнения коллектора носителем
    if (filling_)
    {
        valves[nValves_ - 1].open();
        if (valves[nValves_ - 1].isCommand())
            valveAdjustable.fullyOpen();
    }

    // если заполнили нужный объем
    if (filling_ && (flowmeter.getVolume() >= fillingVolume_))
    {
        // закрываем клапаны после наполнения коллектора носителем
        valveAdjustable.fullyClose();
        if (valveAdjustable.isClosed())
        {
            valves[nValves_ - 1].close();
            filling_ = false;
            fillingFinishing_ = true;
        }
    }

    // ждем несколько секунд для выставления статуса завершения заполнения
    fillingFinishingTimer_.onTimer(fillingFinishing_, 10000);
    if (fillingFinishingTimer_.status)
    {
        fillingFinishing_ = false;
        fillingDone_ = true;
    }
}

void Collector::resetFill()
{
    fillingStart_ = false;
    filling_ = false;
    fillingDone_ = false;
    fillingFinishing_ = false;
}

// дозирование препарата через заданный клапан
void Collector::dose(int valveNum)
{
    // стартуем дозирование
    if (dosingStart_ && !dosing_ && !dosingFinishing_ && !dosingDoneDelay_ && !dosingDone_)
    {
        flowmeter.nullifyVolume();
        dosingVolumeOffset_ = 0.0;
        dosing_ = true;
    }
    dosingStart_ = false;

    // ждем открытия клапана препарата
    dosingValveDelay_.onTimer(dosing_, 5000);

    // сам процесс дозирования
    if (dosing_)
    {
        // открываем клапаны
        valves[valveNum].open();
        if (dosingValveDelay_.status)
            valveAdjustable.fullyOpen();

        // запоминаем смещение объема для определения момента закрытия клапана
        if (dosingValveOpenRise_.rising(valveAdjustable.isOpened()))
            dosingVolumeOffset_ = flowmeter.getVolume();

        // если не успели открыть клапан, а уже половина объема прошла, то заканчиваем дозацию
        if (!valveAdjustable.isOpened() && ((requiredVolumes[valveNum] / 2.0) >= flowmeter.getVolume()))
        {
            dosing_ = false;
            dosingFinishing_ = true;
        }

        // если подходим к завершению дозирования нужного объема
        if ((requiredVolumes[valveNum] - dosingVolumeOffset_) >= flowmeter.getVolume())
        {
            dosing_ = false;
            dosingFinishing_ = true;
        }
    }

    // заканчиваем дозацию закрыванием клапанов
    if (dosingFinishing_)
    {
        valveAdjustable.fullyClose();
        if (valveAdjustable.isClosed())
        {
            valves[valveNum].close();
            dosingFinishing_ = false;
            dosingDoneDelay_ = true;
        }
    }

    // ждем немного после закрытия клапанов
    dosingDoneDelayTimer_.onTimer(dosingDoneDelay_, 10000);
    if (dosingDoneDelayTimer_.status)
    {
        dosingDoneDelay_ = false;
        dosingDone_ = true;
    }

    // сохраняем прошедший объем
    dosedVolumes[valveNum] = flowmeter.getVolume();
}

void Collector::resetDose()
{
    dosing_ = false;
    dosingStart_ = false;
    dosingFinishing_ = false;
    dosingDoneDelay_ = false;
    dosingDone_ = false;
    closeAll();
}

void Collector::wash()
{
    // стартуем с промывку коллектора
    if (washingStart_ && !washing_ && !washingFinishing_ && !washingDone_)
    {
        flowmeter.nullifyVolume();
        washing_ = true;
    }
    washingStart_ = false;

    // открываем клапаны для промывки
    if (washing_)
    {
        // открываем клапаны для промывки коллектора
        valves[nValves_ - 1].open();
        if (valves[nValves_ - 1].isCommand())
            valveAdjustable.fullyOpen();
    }

    // процесс промывки
    washingTimer_.onTimer(washing_ && valveAdjustable.isOpened(), 3000);

    // если заполнили нужный объем
    if (washingTimer_.status)
    {
        // закрываем клапаны после промывки коллектора
        valveAdjustable.fullyClose();
        if (valveAdjustable.isClosed())
        {
            valves[nValves_ - 1].close();
            washing_ = false;
            washingFinishing_ = true;
        }
    }

    // ждем несколько секунд для выставления статуса завершения промывки
    washingFinishingTimer_.onTimer(washingFinishing_, 10000);
    if (washingFinishingTimer_.status)
    {
        washingFinishing_ = false;
        washingDone_ = true;
    }
}

void Collector::resetWash()
{
    washing_ = false;
    washingStart_ = false;
    washingDone_ = false;
    washingFinishing_ = false;
}

void Collector::loopCommand()
{
    resetFill();
    resetDose();
    resetWash();

    loopDone_ = false;
    fillingStart_ = true;
    order = 0;
}

void Collector::loopPause()
{
    loopPause_ = !loopPause_;

    if (loopDone_)
        loopPause_ = false;

    if (loopPause_)
        closeAll();
}

void Collector::loopStop()
{
    fillingDone_ = true;
    dosingDone_ = true;
    washingDone_ = true;
    loopDone_ = true;

    closeAll();
}

void Collector::loop()
{
    // дозирование на паузе
    if (loopPause_)
    {
        return;
    }

    // дозирование завершено
    if (order >= nValves_ - 1)
    {
        loopDone_ = true;
    }

    if (loopDone_)
    {
        return;
    }

    // выполняем дозирование
    fill();
    if (valveNums[order] - 1 != -1)
    {
        dose(valveNums[order]);
        wash();
    }

    if (fillingDone_ || washingDone_)
    {
        resetFill();
        resetWash();
        dosingStart_ = true;
    }
    if (dosingDone_)
    {
        resetDose();
        washingStart_ = true;
        order++;
    }
}