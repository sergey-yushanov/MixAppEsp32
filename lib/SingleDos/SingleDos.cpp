#include "SingleDos.h"

void SingleDos::closeAll()
{
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
void SingleDos::fill()
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
        valveAdjustable.fullyOpen();
    }

    // если заполнили нужный объем
    if (filling_ && (flowmeter.getVolume() >= fillingVolume_))
    {
        // закрываем клапаны после наполнения коллектора носителем
        valveAdjustable.fullyClose();
        if (valveAdjustable.isClosed())
        {
            filling_ = false;
            fillingFinishing_ = true;
        }
    }

    // ждем несколько секунд для выставления статуса завершения заполнения
    fillingFinishingTimer_.on100msTimer(fillingFinishing_, 100);
    if (fillingFinishingTimer_.status)
    {
        fillingFinishing_ = false;
        fillingDone_ = true;
    }
}

void SingleDos::resetFill()
{
    fillingStart_ = false;
    filling_ = false;
    fillingDone_ = false;
    fillingFinishing_ = false;
}

// дозирование препарата
void SingleDos::dose()
{
    // стартуем дозирование
    if (dosingStart_ && !dosing_ && !dosingFinishing_ && !dosingDoneDelay_ && !dosingDone_)
    {
        // flowmeter.nullifyVolume();
        dosingVolumeOffset_ = 0.0;
        dosing_ = true;
        dosingNullify_ = true;

        dosedVolume = 0;

        Serial.print("Dosing start volume: ");
        Serial.println(flowmeter.getVolume());

        Serial.print("Dosing required volume: ");
        Serial.println(requiredVolume);
    }
    dosingStart_ = false;

    // ждем открытия клапана препарата
    dosingValveDelay_.on100msTimer(dosing_, 100);

    // сам процесс дозирования
    if (dosing_)
    {
        // открываем клапаны
        valveAdjustable.fullyOpen();
        dosingNullify_ = false;

        // запоминаем смещение объема для определения момента закрытия клапана
        if (dosingValveOpenRise_.rising(valveAdjustable.isOpened()))
        {
            dosingVolumeOffset_ = flowmeter.getVolume(); // * 1.15;
            Serial.print("Dosing offset volume: ");
            Serial.println(dosingVolumeOffset_);
        }

        // если не успели открыть клапан, а уже половина объема прошла, то заканчиваем дозацию
        if (!valveAdjustable.isOpened() && (dosedVolume >= (requiredVolume / 2.0))) // / 1.15)))
        {
            dosing_ = false;
            dosingFinishing_ = true;

            Serial.print("Dosing volume: ");
            Serial.println(dosedVolume);
        }

        // если подходим к завершению дозирования нужного объема
        if (dosedVolume >= (requiredVolume - dosingVolumeOffset_))
        {
            dosing_ = false;
            dosingFinishing_ = true;

            Serial.print("Dosing volume: ");
            Serial.println(dosedVolume);
        }
    }

    // заканчиваем дозацию закрыванием клапанов
    if (dosingFinishing_)
    {
        valveAdjustable.fullyClose();
        if (valveAdjustable.isClosed())
        {
            dosingFinishing_ = false;
            dosingDoneDelay_ = true;
        }
    }

    // ждем немного после закрытия клапанов
    dosingDoneDelayTimer_.on100msTimer(dosingDoneDelay_, 100);
    if (dosingDoneDelayTimer_.status)
    {
        dosingDoneDelay_ = false;
        dosingDone_ = true;
    }

    // сохраняем прошедший объем
    if (dosingNullify_)
        flowmeter.nullifyVolume();

    if (dosing_ || dosingFinishing_)
        dosedVolume = flowmeter.getVolume();
}

void SingleDos::resetDose()
{
    dosing_ = false;
    dosingStart_ = false;
    dosingFinishing_ = false;
    dosingDoneDelay_ = false;
    dosingDone_ = false;
    dosingNullify_ = false;

    closeAll();
}

void SingleDos::wash()
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
        valveAdjustable.fullyOpen();
    }

    // процесс промывки
    washingTimer_.on100msTimer(washing_ && valveAdjustable.isOpened(), 30);

    // если заполнили нужный объем
    if (washingTimer_.status)
    {
        washing_ = false;
        washingFinishing_ = true;

        // закрываем клапаны после промывки коллектора
        valveAdjustable.fullyClose();
    }

    // ждем несколько секунд для выставления статуса завершения промывки
    if (washingFinishing_ && valveAdjustable.isClosed())
    {
        washingFinishing_ = false;
        washingDone_ = true;
    }
}

void SingleDos::resetWash()
{
    washing_ = false;
    washingStart_ = false;
    washingDone_ = false;
    washingFinishing_ = false;
}

void SingleDos::loopStart()
{
    resetFill();
    resetDose();
    resetWash();

    loopDone_ = false;
    fillingStart_ = true;
    order = 0;

    dosedVolume = 0;
}

void SingleDos::loopStop()
{
    resetFill();
    resetDose();
    resetWash();

    loopDone_ = false;
    order = 0;

    closeAll();
}

void SingleDos::loopPause()
{
    // loopPause_ = !loopPause_;

    // if (loopDone_)
    //     loopPause_ = false;

    // if (loopPause_)
    //     closeAll();
}

// void Collector::doseCommand()
// {
//     resetFill();
//     resetDose();
//     resetWash();

//     loopDone_ = false;
//     dosingStart_ = true;
// }

void SingleDos::loop()
{
    // дозирование на паузе
    // if (loopPause_)
    // {
    //     return;
    // }

    if (loopDone_)
    {
        return;
    }

    // выполняем дозирование
    fill();
    dose();
    wash();

    if (fillingDone_ || washingDone_)
    {
        resetFill();
        resetWash();

        // дозирование завершено
        loopDone_ = true;

        // Serial.print("order: ");
        // Serial.print(order);
        // Serial.print("\tvalveNum: ");
        // Serial.println(valveNums[order]);
    }

    if (dosingDone_)
    {
        resetDose();
        washingStart_ = true;
        order++;
    }
}

void SingleDos::incTimers()
{
    fillingFinishingTimer_.inc100msTimer();

    dosingValveDelay_.inc100msTimer();
    dosingDoneDelayTimer_.inc100msTimer();

    washingTimer_.inc100msTimer();
    washingFinishingTimer_.inc100msTimer();
}

void SingleDos::setRatioVolume(float ratioVolume)
{
    ratioVolume_ = ratioVolume;
}

void SingleDos::setRatioVolumeMicro(float ratioVolumeMicro)
{
    ratioVolumeMicro_ = ratioVolumeMicro;
}