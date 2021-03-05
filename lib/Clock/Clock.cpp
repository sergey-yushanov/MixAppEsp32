#include <Clock.h>

void Clock::set()
{
    _0_1s = true;
    counter++;
    if (counter % 2 == 0)
        _0_2s = true;
    if (counter % 5 == 0)
        _0_5s = true;
    if (counter % 10 == 0)
        _1s = true;
    if (counter % 20 == 0)
        _2s = true;
    if (counter % 50 == 0)
        _5s = true;
    if (counter % 100 == 0)
    {
        _10s = true;
        counter = 0;
    }
}

void Clock::reset()
{
    _0_1s = false;
    _0_2s = false;
    _0_5s = false;
    _1s = false;
    _2s = false;
    _5s = false;
    _10s = false;
}