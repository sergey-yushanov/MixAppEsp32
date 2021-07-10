#ifndef PLANT_h
#define PLANT_h

#include <DispenserCollector.h>
#include <Flowmeter.h>
#include <ValveAdjustable.h>

#include "save_data.h"

extern DispenserCollector dispenserCollector;
extern Flowmeter m1;
extern ValveAdjustable valveAdjustable;

extern bool ack;
extern bool showSettings;

// void m1Setup();
void IRAM_ATTR m1Pulse();

// void g1Setup();
void IRAM_ATTR g1Pulse();

void incTimeouts();
void plantSetup();
void plantLoop();
// void loopPlant();

#endif