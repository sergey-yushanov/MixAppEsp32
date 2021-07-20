#ifndef PLANT_h
#define PLANT_h

#include <Collector.h>
#include <Flowmeter.h>
#include <ValveAdjustable.h>

#include "save_data.h"

extern Collector collector;
extern Flowmeter m1;
extern ValveAdjustable valveAdjustable;

extern bool ack;
extern bool showSettings;
extern bool pumpCommand;
extern bool loopCommand;

// void m1Setup();
void IRAM_ATTR m1Pulse();

// void g1Setup();
void IRAM_ATTR g1Pulse();

void incTimeouts();
void plantSetup();
void plantLoop();
void flowLoop();
void mixLoop();

#endif