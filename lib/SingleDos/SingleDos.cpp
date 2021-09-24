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
        dosingNullify_ = false;

        // запоминаем смещение объема для определения момента закрытия клапана
        if (dosingValveOpenRise_.rising(valveAdjustable.isOpened()))
        {
            dosingVolumeOffset_ = flowmeter.getVolume() * dosingVolumeOffsetRatio_;
            Serial.print("Single dos Dosing offset volume: ");
            Serial.println(dosingVolumeOffset_);
        }

        // если не успели открыть клапан, а уже половина объема прошла, то заканчиваем дозацию
        if (!valveAdjustable.isOpened() && ((dosedVolume * dosingVolumeOffsetRatio_) >= (requiredVolume / 2.0)))
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

void SingleDos::setRatioVolume(float ratioVolume)
{
    ratioVolume_ = ratioVolume;
}

void SingleDos::setRatioVolumeMicro(float ratioVolumeMicro)
{
    ratioVolumeMicro_ = ratioVolumeMicro;
}