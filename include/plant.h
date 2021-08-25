#ifndef PLANT_h
#define PLANT_h

#include <Collector.h>
#include <Flowmeter.h>
#include <ValveAdjustable.h>

#include "save_data.h"

#include "clock_pulses.h"
#include <ProcessControl.h>

extern Collector collector;
extern Flowmeter m1;
extern ValveAdjustable valveAdjustable;
extern float carrierRequiredVolume;
extern float carrierDosedVolume;

extern bool ack;
extern bool showSettings;
extern bool pumpCommand;
// extern bool loopCommand;

extern long openTime;
extern long openTimeBegin;
extern long openTimeEnd;
extern bool openTimeCalc;
extern float openPosition;

extern long closeTime;
extern long closeTimeBegin;
extern long closeTimeEnd;
extern bool closeTimeCalc;

// void m1Setup();
void IRAM_ATTR m1Pulse();

// void g1Setup();
void IRAM_ATTR g1Pulse();

void incTimeouts();
void incTimers();
void plantSetup();
void plantLoop();
void flowLoop();
void mixLoop();

void loopStart();
void loopStop();
void loopPause();

#endif