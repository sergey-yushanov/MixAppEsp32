#include <Collector.h>

void Collector::closeAll()
{
    for (int i = 0; i < nValves_; i++)
        valves[i].close();
    valveAdjustable.fullyClose();
}

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
    fillingFinishingTimer_.on100msTimer(fillingFinishing_, 20);
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
void Collector::dose() // int valveNum)
{
    // стартуем дозирование
    if (dosingStart_ && !dosing_ && !dosingFinishing_ && !dosingDoneDelay_ && !dosingDone_)
    {
        // flowmeter.nullifyVolume();
        dosingVolumeOffset_ = 0.0;
        dosing_ = true;
        dosingNullify_ = true;

        dosedVolumes[order] = 0;

        Serial.print("ValveNum: ");
        Serial.println(valveNum);

        // Serial.print("Dosing start volume: ");
        // Serial.println(flowmeter.getVolume());

        Serial.print("Dosing required volume: ");
        Serial.println(requiredVolumes[order]);

        dosingVolume2_ = requiredVolumes[order] <= volume2_;
        // dosingVolume1_ = !dosingVolume2_ && (requiredVolumes[order] <= volume1_);
        dosingVolume1_ = (requiredVolumes[order] > volume2_) && (requiredVolumes[order] <= volume1_);
    }
    dosingStart_ = false;

    // ждем открытия клапана препарата
    dosingValveDelay_.on100msTimer(dosing_, 20);

    // сам процесс дозирования
    if (dosing_)
    {
        // открываем клапаны
        valves[valveNum].open();
        if (dosingValveDelay_.status)
        {
            valveAdjustable.fullyOpen();
            if (dosingVolume1_)
                valveAdjustable.setSetpoint(setpoint1_);
            if (dosingVolume2_)
                valveAdjustable.setSetpoint(setpoint2_);

            dosingNullify_ = false;
        }

        // запоминаем смещение объема для определения момента закрытия клапана
        // if (dosingValveOpenRise_.rising(valveAdjustable.isOpened()))
        if ((dosingVolumeOffset_ == 0) && dosingValveOpenRise_.rising(valveAdjustable.isPositionOk()))
        {
            // if (dosingMicro_)
            // {
            //     dosingVolumeOffset_ = flowmeter.getVolume() * ratioVolumeMicro_;
            // }
            // else
            // {
            //     dosingVolumeOffset_ = flowmeter.getVolume() * ratioVolume_;
            // }
            dosingVolumeOffset_ = dosedVolumeWithRatio_;
            Serial.print("Dosing offset volume: ");
            Serial.println(dosingVolumeOffset_);
        }

        // если не успели открыть клапан, а уже половина объема прошла, то заканчиваем дозацию
        // if (!valveAdjustable.isOpened() && (dosedVolumes[order] >= (requiredVolumes[order] / 2.0))) // / 1.15)))
        if (!valveAdjustable.isPositionOk() && (dosedVolumeWithRatio_ >= (requiredVolumes[order] / 2.0)))
        {
            dosing_ = false;
            dosingFinishing_ = true;

            Serial.print("Dosing volume: ");
            Serial.println(dosedVolumes[order]);
        }

        // если подходим к завершению дозирования нужного объема
        if (dosedVolumes[order] >= (requiredVolumes[order] - dosingVolumeOffset_))
        {
            dosing_ = false;
            dosingFinishing_ = true;

            Serial.print("Dosing volume: ");
            Serial.println(dosedVolumes[order]);
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
    dosingDoneDelayTimer_.on100msTimer(dosingDoneDelay_, 20);
    if (dosingDoneDelayTimer_.status)
    {
        dosingDoneDelay_ = false;
        dosingDone_ = true;
    }

    // сохраняем прошедший объем
    if (dosingNullify_)
        flowmeter.nullifyVolume();

    if (dosing_ || dosingFinishing_)
        dosedVolumes[order] = flowmeter.getVolume();

    dosedVolumeWithRatio_ = dosedVolumes[order] * ratioVolume0_;
    if (dosingVolume1_)
        dosedVolumeWithRatio_ = dosedVolumes[order] * ratioVolume1_;
    if (dosingVolume2_)
        dosedVolumeWithRatio_ = dosedVolumes[order] * ratioVolume2_;
}

void Collector::resetDose()
{
    dosing_ = false;
    dosingStart_ = false;
    dosingFinishing_ = false;
    dosingDoneDelay_ = false;
    dosingDone_ = false;
    dosingNullify_ = false;

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
    washingTimer_.on100msTimer(washing_ && valveAdjustable.isOpened(), 10);

    // если заполнили нужный объем
    if (washingTimer_.status && !washingCarrierReserve_)
    // if (washing_ && valveAdjustable.isOpened() && !washingCarrierReserve_)
    {
        washing_ = false;
        washingFinishing_ = true;

        // закрываем клапаны после промывки коллектора
        valveAdjustable.fullyClose();
    }

    // todo: здесь сделать так, чтобы время выставления статуса не влияло на окончание закрытия клапана
    if (washingFinishing_ && valveAdjustable.isClosed())
    {
        valves[nValves_ - 1].close();
    }

    // ждем несколько секунд для выставления статуса завершения промывки
    washingFinishingTimer_.on100msTimer(washingFinishing_ && valveAdjustable.isClosed(), 20);
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

    washingCarrierReserve_ = false;
}

void Collector::loopStart()
{
    resetFill();
    resetDose();
    resetWash();

    fillingStart_ = true;
    loopDone_ = false;
    order = 0;

    // обнуляем объемы
    for (int i = 0; i < nValves_ - 2; i++)
    {
        dosedVolumes[i] = 0;
    }
}

void Collector::loopStop()
{
    resetFill();
    resetDose();
    resetWash();

    loopDone_ = false;
    order = -1;

    closeAll();
}

void Collector::loopPause()
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

void Collector::loop()
{
    // if (clk._100ms)
    // {
    // Serial.print("Order: ");
    // Serial.print(order);
    // Serial.print("\tvalveNum: ");
    // Serial.println(valveNum);
    // }
    // дозирование на паузе
    // if (loopPause_)
    // {
    //     return;
    // }

    if (washingCarrierReserve_) // && (order == -1))
    {
        // washingStart_ = true;
        wash();
        return;
    }

    if (loopDone_)
    {
        Serial.println("Loop collector Done!");
        loopStop();
        return;
    }

    if (order == -1)
    {
        return;
    }

    // активный клапан для дозации
    valveNum = valveNums[order] - 1;
    if (valveNum == -1)
    {
        loopDone_ = true;
        return;
    }

    // выполняем дозирование
    fill();
    dose(); // valveNums[order] - 1);
    wash();

    if (fillingDone_)
    {
        resetFill();
        dosingStart_ = true;
    }

    if (washingDone_)
    {
        resetWash();

        // дозирование завершено
        if (order >= nValves_ - 2)
        {
            loopDone_ = true;
            return;
            // order = -1;
        }
        else
        {
            dosingStart_ = true;
            order++;
        }
    }

    if (dosingDone_)
    {
        resetDose();
        // убираем промежуточную промывку, выставляем сразу Done
        // washingStart_ = true;
        washingDone_ = true;
        // order++;
    }
}

void Collector::incTimers()
{
    fillingFinishingTimer_.inc100msTimer();

    dosingValveDelay_.inc100msTimer();
    dosingDoneDelayTimer_.inc100msTimer();

    washingTimer_.inc100msTimer();
    washingFinishingTimer_.inc100msTimer();
}

void Collector::setRatioVolume0(float ratioVolume)
{
    ratioVolume0_ = ratioVolume;
}

void Collector::setRatioVolume1(float ratioVolume)
{
    ratioVolume1_ = ratioVolume;
}

void Collector::setRatioVolume2(float ratioVolume)
{
    ratioVolume2_ = ratioVolume;
}

// void Collector::setRatioVolumeMicro(float ratioVolumeMicro)
// {
//     ratioVolumeMicro_ = ratioVolumeMicro;
// }

void Collector::setVolume1(float volume)
{
    volume1_ = volume;
}

void Collector::setVolume2(float volume)
{
    volume2_ = volume;
}

// void Collector::setMicroVolume(float microVolume)
// {
//     microVolume_ = microVolume;
// }

void Collector::setSetpoint1(float setpoint)
{
    setpoint1_ = setpoint;
}

void Collector::setSetpoint2(float setpoint)
{
    setpoint2_ = setpoint;
}

// void Collector::setMicroSetpoint(float microSetpoint)
// {
//     microSetpoint_ = microSetpoint;
// }

float Collector::getDosedVolume()
{
    float sum = 0;
    for (int i = 0; i < nValves_ - 1; i++)
        sum += dosedVolumes[i];
    return sum;
}