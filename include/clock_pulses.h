#ifndef CLOCK_PULSES_h
#define CLOCK_PULSES_h

#include <Clock.h>

extern Clock clk;

void IRAM_ATTR onTime();
void clockSetup();
void clockReset();

#endif