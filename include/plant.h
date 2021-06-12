#ifndef PLANT_h
#define PLANT_h

#include <DispenserCollector.h>
#include <Flowmeter.h>
#include <ValveAdjustable.h>

extern DispenserCollector dispenserCollector;
extern Flowmeter m1;
extern ValveAdjustable valveAdjustable;
// extern bool ack;

void m1Setup();
void IRAM_ATTR m1Pulse();

void g1Setup();
void IRAM_ATTR g1Pulse();

void loopPlant();

#endif