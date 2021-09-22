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

    void setRatioVolume(float ratioVolume);
    void setRatioVolumeMicro(float ratioVolumeMicro);
};

#endif