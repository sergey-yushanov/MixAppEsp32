#ifndef SENSORS_h
#define SENSORS_h

#include <AnalogSensor.h>
#include <CD74HC4067.h>

extern const int nAnalogSensors;
extern CD74HC4067 analogMux;
extern AnalogSensor analogSensors[];

void analogSensorsSetup();
void analogSensorsRead();

#endif