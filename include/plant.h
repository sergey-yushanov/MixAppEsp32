#ifndef PLANT_h
#define PLANT_h

#include "esp_adc_cal.h"

#include <Collector.h>
#include <Flowmeter.h>
#include <ValveAdjustable.h>
#include "SingleDos.h"

#include "save_data.h"

#include "clock_pulses.h"
#include <ProcessControl.h>

static const int nCollectors = 2;
extern Collector collectors[nCollectors];
extern Flowmeter m1;
extern ValveAdjustable valveAdjustable;
// extern SingleDos singleDos;

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

// loop parameters
extern float ratioVolume;
extern float ratioVolumeMicro;
extern float valveSetpoint;
extern float carrierReserve;

// flow meters
extern float banjoDefaultPulsesPerLiter;
extern float darkontDefaultPulsesPerLiter;

// void m1Setup();
void IRAM_ATTR m1Pulse();

// void g1Setup();
void IRAM_ATTR g1Pulse();
void IRAM_ATTR g2Pulse();

void incTimeouts();
void incTimers();
void plantSetup();
void plantLoop();
void flowLoop();
void flowReady();
void mixLoop();

void loopStart();
void loopStop();
void loopPause();

void loopDevicesStop();
void loopCollectorWashingStart();
void loopCollectorWashingStop();

void pumpStart();
void pumpStop();

#endif