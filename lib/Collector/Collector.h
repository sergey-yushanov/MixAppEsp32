#ifndef COLLECTOR_h
#define COLLECTOR_h

#include <Valve.h>
#include <ValveAdjustable.h>
#include <Flowmeter.h>
#include <ProcessControl.h>

// #include "clock_pulses.h"

struct Collector
{
    static const int nValves_ = 4;
    Valve valves[nValves_];
    ValveAdjustable valveAdjustable;
    Flowmeter flowmeter;

    int valveNums[nValves_ - 1];
    float requiredVolumes[nValves_ - 1];
    float dosedVolumes[nValves_ - 1];

    bool applyCommand;

    // наполнение
    bool filling_;
    bool fillingStart_;
    bool fillingFinishing_;
    bool fillingDone_;
    float fillingVolume_ = 5.0;
    OnTimer fillingFinishingTimer_;

    // дозирование
    bool dosing_;
    bool dosingStart_;
    bool dosingFinishing_;
    bool dosingDoneDelay_;
    bool dosingDone_;
    bool dosingNullify_;
    RisingEdge dosingValveOpenRise_;
    float dosingVolumeOffset_;
    OnTimer dosingValveDelay_;
    OnTimer dosingDoneDelayTimer_;

    // коэффициент для умножения значения объема при открытии клапана
    // float dosingVolumeOffsetRatio_ = 1.15;

    // микродозация
    bool dosingVolume1_;
    bool dosingVolume2_;

    float volume1_ = 15.0;
    float volume2_ = 3.5;

    float setpoint1_ = 50.0;
    float setpoint2_ = 25.0;

    // прикрываем клапан для точной дозации
    bool dosingAccuracy_;
    float dosingAccuracyVolume_;
    float dosingAccuracyPosition_;

    // промывка
    bool washing_;
    bool washingStart_;
    bool washingDone_;
    bool washingFinishing_;

    bool washingCarrierReserve_ = false;
    // float washingVolume_;
    OnTimer washingTimer_;
    OnTimer washingFinishingTimer_;

    RisingEdge commandRise_;

    int order;
    int valveNum;

    bool loopDone_;
    bool loopPause_;

    bool loopUsing_;

    // loop parameters
    float ratioVolume0_ = 1.2;
    float ratioVolume1_ = 1.56;
    float ratioVolume2_ = 2.2;
    float dosedVolumeWithRatio_;

    void closeAll();
    // void stopFill();
    // void openToFill();
    // void closeAfterFilling();
    // void closeAfterDosing(int valveNum);
    // void openToWash();

    void fill();
    void resetFill();
    void dose(); // int valveNum);
    void resetDose();
    void wash();
    void resetWash();

    void loop();
    void loopStart();
    void loopStop();
    void loopPause();

    void fillCommand();
    void doseCommand();
    void washCommand();

    void incTimers();

    void setRatioVolume0(float ratioVolume);
    void setRatioVolume1(float ratioVolume);
    void setRatioVolume2(float ratioVolume);

    // void setRatioVolume(float ratioVolume);
    // void setRatioVolumeMicro(float ratioVolumeMicro);

    void setVolume1(float volume);
    void setVolume2(float volume);
    // void setMicroVolume(float microVolume);

    void setSetpoint1(float setpoint);
    void setSetpoint2(float setpoint);
    // void setMicroSetpoint(float microSetpoint);

    float getDosedVolume();
};

#endif