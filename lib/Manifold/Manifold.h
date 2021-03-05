#ifndef MANIFOLD_h
#define MANIFOLD_h

#include <Valve.h>
#include <Flowmeter.h>

struct Manifold
{
    static const int nValves_ = 5;
    Valve valves[nValves_];
    Flowmeter flowmeter;
};

#endif