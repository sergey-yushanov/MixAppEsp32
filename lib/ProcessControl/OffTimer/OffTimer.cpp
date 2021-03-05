#include "OffTimer.h"

OffTimer::OffTimer()
{
}

void OffTimer::offTimer(bool enable, long intervalMillis)
{
    if (enable)
    {
        startMillis_ = millis();
        intervalMillis_ = intervalMillis;
    }

    passedMillis_ = millis() - startMillis_;
    status = enable || (passedMillis_ <= intervalMillis_);
}