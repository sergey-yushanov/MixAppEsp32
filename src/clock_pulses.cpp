#include "clock_pulses.h"

Clock clk;

void IRAM_ATTR onTime()
{
    portENTER_CRITICAL_ISR(&clk.timerMux);
    clk.set();
    portEXIT_CRITICAL_ISR(&clk.timerMux);
}

void clockSetup()
{
    clk.timer = timerBegin(0, 80, true);
    timerAttachInterrupt(clk.timer, onTime, true);
    timerAlarmWrite(clk.timer, 10000, true); // 10ms clk
    timerAlarmEnable(clk.timer);
}

void clockReset()
{
    portENTER_CRITICAL(&clk.timerMux);
    clk.reset();
    portEXIT_CRITICAL(&clk.timerMux);
}