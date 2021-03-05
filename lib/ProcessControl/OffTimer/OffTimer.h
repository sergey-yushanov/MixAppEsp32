#ifndef OFF_TIMER_h
#define OFF_TIMER_h

#include <Arduino.h>

class OffTimer
{
private:
    long intervalMillis_;
    long startMillis_;
    long passedMillis_;

public:
    bool status;

    OffTimer();
    void offTimer(bool enable, long intervalMillis);
};

#endif