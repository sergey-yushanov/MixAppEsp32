#include "SingleDos.h"

void SingleDos::closeAll()
{
    valveAdjustable.fullyClose();
}

// дозирование препарата
void SingleDos::dose()
{
    // стартуем дозирование
    if (dosingStart_ && !dosing_ && !dosingFinishing_ && !dosingDone_)
    {
        dosingVolumeOffset_ = 0.0;
        dosing_ = true;
        dosingNullify_ = true;

        dosedVolume = 0;

        Serial.print("Dosing required volume: ");
        Serial.println(requiredVolume);

        dosingVolume2_ = requiredVolume <= volume2_;
        dosingVolume1_ = (requiredVolume > volume2_) && (requiredVolume <= volume1_);
    }
    dosingStart_ = false;

    // correcting flow timer
    correctFlowTimer_.on100msTimer(!correctFlow_, 100);
    if (correctFlowTimer_.status)
    {
        correctFlow_ = true;
    }

    // сам процесс дозирования
    if (dosing_)
    {
        // открываем клапаны
        valveAdjustable.fullyOpen();

        if (dosingVolume1_)
            valveAdjustable.setSetpoint(setpoint1_);
        if (dosingVolume2_)
            valveAdjustable.setSetpoint(setpoint2_);

        dosingNullify_ = false;

        // запоминаем смещение объема для определения момента закрытия клапана
        if ((dosingVolumeOffset_ == 0) && dosingValveOpenRise_.rising(valveAdjustable.isPositionOk()))
        {
            dosingVolumeOffset_ = dosedVolumeWithRatio_;
            Serial.print("Single dos Dosing offset volume: ");
            Serial.println(dosingVolumeOffset_);
        }

        // если не успели открыть клапан, а уже половина объема прошла, то заканчиваем дозацию
        if (!valveAdjustable.isPositionOk() && (dosedVolumeWithRatio_ >= (requiredVolume / 2.0)))
        {
            dosing_ = false;
            dosingFinishing_ = true;

            Serial.print("Single dos Dosing volume: ");
            Serial.println(dosedVolume);
        }

        // если подходим к завершению дозирования нужного объема
        if (dosedVolume >= (requiredVolume - dosingVolumeOffset_))
        {
            dosing_ = false;
            dosingFinishing_ = true;

            Serial.print("Single dos Dosing volume: ");
            Serial.println(dosedVolume);
        }

        // need correcting flow to flowmeter limit
        if (correctFlow_ && (flowmeter.getFlow() > flowmeter.getLimitFlow()))
        {
            Serial.println("Single dos correct flow");
            Serial.println(valveAdjustable.getSetpoint());
            valveAdjustable.setSetpoint(valveAdjustable.getSetpoint() - 10.0);
            Serial.println(valveAdjustable.getSetpoint());
            correctFlow_ = false;
        }
    }

    // заканчиваем дозацию закрыванием клапанов
    if (dosingFinishing_)
    {
        valveAdjustable.fullyClose();
        if (valveAdjustable.isClosed())
        {
            dosingFinishing_ = false;
            dosingDone_ = true;
        }
    }

    // сохраняем прошедший объем
    if (dosingNullify_)
        flowmeter.nullifyVolume();

    if (dosing_ || dosingFinishing_)
        dosedVolume = flowmeter.getVolume();

    dosedVolumeWithRatio_ = dosedVolume * ratioVolume0_;
    if (dosingVolume1_)
        dosedVolumeWithRatio_ = dosedVolume * ratioVolume1_;
    if (dosingVolume2_)
        dosedVolumeWithRatio_ = dosedVolume * ratioVolume2_;
}

void SingleDos::resetDose()
{
    dosing_ = false;
    dosingStart_ = false;
    dosingFinishing_ = false;
    dosingDone_ = false;
    dosingNullify_ = false;

    closeAll();
}

void SingleDos::loopStart()
{
    resetDose();

    dosingStart_ = true;
    loopDone_ = false;
    dosedVolume = 0;
    flowmeter.nullifyVolume();
}

void SingleDos::loopStop()
{
    resetDose();

    loopDone_ = false;
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

void SingleDos::incTimers()
{
    correctFlowTimer_.inc100msTimer();
}

void SingleDos::loop()
{
    // дозирование на паузе
    // if (loopPause_)
    // {
    //     return;
    // }

    if (loopDone_)
    {
        Serial.println("Loop single dos Done!");
        loopStop();
        return;
    }

    // выполняем дозирование
    dose();

    // дозирование завершено
    if (dosingDone_)
    {
        resetDose();
        loopDone_ = true;
    }
}

void SingleDos::setRatioVolume0(float ratioVolume)
{
    ratioVolume0_ = ratioVolume;
}

void SingleDos::setRatioVolume1(float ratioVolume)
{
    ratioVolume1_ = ratioVolume;
}

void SingleDos::setRatioVolume2(float ratioVolume)
{
    ratioVolume2_ = ratioVolume;
}

// void Collector::setRatioVolumeMicro(float ratioVolumeMicro)
// {
//     ratioVolumeMicro_ = ratioVolumeMicro;
// }

void SingleDos::setVolume1(float volume)
{
    volume1_ = volume;
}

void SingleDos::setVolume2(float volume)
{
    volume2_ = volume;
}

// void Collector::setMicroVolume(float microVolume)
// {
//     microVolume_ = microVolume;
// }

void SingleDos::setSetpoint1(float setpoint)
{
    setpoint1_ = setpoint;
}

void SingleDos::setSetpoint2(float setpoint)
{
    setpoint2_ = setpoint;
}