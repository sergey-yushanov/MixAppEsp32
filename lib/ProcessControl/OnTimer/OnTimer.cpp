#include "OnTimer.h"

OnTimer::OnTimer()
{
}

void OnTimer::onTimer(bool enable, long intervalMillis)
{
    if (!enable)
    {
        startMillis_ = millis();
        intervalMillis_ = intervalMillis;
    }

    passedMillis_ = millis() - startMillis_;
    status = enable && (passedMillis_ >= intervalMillis_);
}