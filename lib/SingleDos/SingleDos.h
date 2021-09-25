#ifndef SINGLE_DOS_h
#define SINGLE_DOS_h

#include <ValveAdjustable.h>
#include <Flowmeter.h>
#include <ProcessControl.h>

struct SingleDos
{
    ValveAdjustable valveAdjustable;
    Flowmeter flowmeter;

    float requiredVolume;
    float dosedVolume;

    // bool applyCommand;

    // микродозация
    bool dosingVolume1_;
    bool dosingVolume2_;

    float volume1_ = 15.0;
    float volume2_ = 5.0;

    float setpoint1_ = 50.0;
    float setpoint2_ = 25.0;

    bool dosing_;
    bool dosingStart_;
    bool dosingFinishing_;
    bool dosingDone_;
    bool dosingNullify_;
    RisingEdge dosingValveOpenRise_;
    float dosingVolumeOffset_;

    float dosingVolumeOffsetRatio_ = 1.15;

    RisingEdge commandRise_;

    bool loopDone_;
    bool loopPause_;

    bool loopUsing_;

    // loop parameters
    float ratioVolume0_ = 1.0;
    float ratioVolume1_ = 1.56;
    float ratioVolume2_ = 2.2;
    float dosedVolumeWithRatio_;

    OnTimer correctFlowTimer_;
    bool correctFlow_;

    // loop parameters
    float ratioVolume_;
    float ratioVolumeMicro_;

    void closeAll();
    // void stopFill();
    // void openToFill();
    // void closeAfterFilling();
    // void closeAfterDosing(int valveNum);
    // void openToWash();

    void dose();
    void resetDose();

    void loop();
    void loopStart();
    void loopStop();
    void loopPause();

    void doseCommand();

    void incTimers();

    void setRatioVolume0(float ratioVolume);
    void setRatioVolume1(float ratioVolume);
    void setRatioVolume2(float ratioVolume);

    void setVolume1(float volume);
    void setVolume2(float volume);

    void setSetpoint1(float setpoint);
    void setSetpoint2(float setpoint);
};

#endif