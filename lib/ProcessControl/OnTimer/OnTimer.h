#ifndef ON_TIMER_h
#define ON_TIMER_h

#include <Arduino.h>

class OnTimer
{
private:
    long intervalMillis_;
    long startMillis_;
    long passedMillis_;

public:
    bool status;

    OnTimer();
    void onTimer(bool enable, long intervalMillis);
};

#endif