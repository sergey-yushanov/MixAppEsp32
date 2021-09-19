#include "OnTimer.h"

OnTimer::OnTimer()
{
}

// void OnTimer::onTimer(bool enable, long intervalMillis)
// {
//     if (!enable)
//     {
//         startMillis_ = millis();
//         intervalMillis_ = intervalMillis;
//     }

//     passedMillis_ = millis() - startMillis_;
//     status = enable && (passedMillis_ >= intervalMillis_);
// }

void OnTimer::on100msTimer(bool enable, int counter100ms)
{
    enable_ = enable;
    status = enable && (counter_ >= counter100ms);
}

void OnTimer::inc100msTimer()
{
    if (!status)
        counter_++;

    if (!enable_)
        counter_ = 0;
}