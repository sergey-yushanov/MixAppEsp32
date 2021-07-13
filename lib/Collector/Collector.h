#ifndef COLLECTOR_h
#define COLLECTOR_h

#include <Valve.h>
#include <ValveAdjustable.h>
#include <Flowmeter.h>

struct Collector
{
    static const int nValves_ = 4;
    Valve valves[nValves_];
    ValveAdjustable valveAdjustable;
    Flowmeter flowmeter;
};

#endif