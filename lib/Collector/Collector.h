#ifndef COLLECTOR_h
#define COLLECTOR_h

#include <Valve.h>
#include <ValveAdjustable.h>
#include <Flowmeter.h>
#include <ProcessControl.h>

struct Collector
{
    static const int nValves_ = 4;
    Valve valves[nValves_];
    ValveAdjustable valveAdjustable;
    Flowmeter flowmeter;

    int valveNums[nValves_ - 1];
    float requiredVolumes[nValves_ - 1];
    float dosedVolumes[nValves_ - 1];

    bool filling_;
    bool fillingStart_;
    bool fillingDone_;
    bool fillingFinishing_;
    float fillingVolume_ = 5.0;
    OnTimer fillingFinishingTimer_;

    bool dosing_;
    bool dosingStart_;
    bool dosingFinishing_;
    bool dosingDoneDelay_;
    bool dosingDone_;
    RisingEdge dosingValveOpenRise_;
    float dosingVolumeOffset_;
    OnTimer dosingValveDelay_;
    OnTimer dosingDoneDelayTimer_;

    bool washing_;
    bool washingStart_;
    bool washingDone_;
    bool washingFinishing_;
    // float washingVolume_;
    OnTimer washingTimer_;
    OnTimer washingFinishingTimer_;

    RisingEdge commandRise_;

    int order;
    bool loopDone_;
    bool loopPause_;

    void closeAll();
    // void stopFill();
    // void openToFill();
    // void closeAfterFilling();
    // void closeAfterDosing(int valveNum);
    // void openToWash();

    void fill();
    void resetFill();
    void dose(int valveNum);
    void resetDose();
    void wash();
    void resetWash();

    void loop(bool command);
};

#endif