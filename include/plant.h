#ifndef PLANT_h
#define PLANT_h

#include <Manifold.h>
#include <Flowmeter.h>
#include <Valve.h>

extern Manifold manifold;
extern Flowmeter m1;
extern Valve valveControl;
extern bool ack;

void m1Setup();
void IRAM_ATTR m1Pulse();

void g1Setup();
void IRAM_ATTR g1Pulse();

void loopPlant();

#endif