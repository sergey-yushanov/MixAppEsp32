#include <Clock.h>

void Clock::set()
{
    _10ms = true;
    counter++;

    if (counter % 2 == 0)
        _20ms = true;
    if (counter % 5 == 0)
        _50ms = true;

    if (counter % 10 == 0)
        _100ms = true;
    if (counter % 20 == 0)
        _200ms = true;
    if (counter % 50 == 0)
        _500ms = true;

    if (counter % 100 == 0)
        _1s = true;
    if (counter % 200 == 0)
        _2s = true;
    if (counter % 500 == 0)
        _5s = true;

    if (counter % 1000 == 0)
    {
        _10s = true;
        counter = 0;
    }
}

void Clock::reset()
{
    _10ms = false;
    _20ms = false;
    _50ms = false;
    _100ms = false;
    _200ms = false;
    _500ms = false;
    _1s = false;
    _2s = false;
    _5s = false;
    _10s = false;
}