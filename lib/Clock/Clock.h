#ifndef CLOCK_h
#define CLOCK_h

#include <Arduino.h>

struct Clock
{
    volatile bool _10ms = false;
    volatile bool _20ms = false;
    volatile bool _50ms = false;
    volatile bool _100ms = false;
    volatile bool _200ms = false;
    volatile bool _500ms = false;
    volatile bool _1s = false;
    volatile bool _2s = false;
    volatile bool _5s = false;
    volatile bool _10s = false;
    volatile int counter = 0;

    hw_timer_t *timer = NULL;
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

    void set();
    void reset();
};

#endif