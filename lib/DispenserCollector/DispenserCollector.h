#ifndef DISPENSER_COLLECTOR_h
#define DISPENSER_COLLECTOR_h

#include <Valve.h>
#include <ValveAdjustable.h>
#include <Flowmeter.h>

struct DispenserCollector
{
    static const int nValves_ = 4;
    Valve valves[nValves_];
    ValveAdjustable valveAdjustable;
    Flowmeter flowmeter;

    void setup();
    void loop();
};

#endif