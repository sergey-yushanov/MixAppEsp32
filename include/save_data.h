#ifndef SAVE_DATA_h
#define SAVE_DATA_h

#include <Preferences.h>

#include "plant.h"

extern const char *preferences_common;
extern const char *preferences_dispenser_0;

void dataSetup();
void dataSave(const char *key, ValveAdjustable *valveAdjustable);
void dataSave(const char *key, Flowmeter *flowmeter);

#endif