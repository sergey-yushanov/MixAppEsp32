#include "sensors.h"

// Analog inputs
const int analogMuxS0 = 26;
const int analogMuxS1 = 25;
const int analogMuxS2 = 33;
const int analogMuxS3 = 32;
const int analogMuxPin = 35;
const int nAnalogSensors = 6;

// multiplexer
CD74HC4067 analogMux(analogMuxS0, analogMuxS1, analogMuxS2, analogMuxS3);
AnalogSensor analogSensors[nAnalogSensors];

void analogSensorsSetup()
{
    for (int i = 0; i < nAnalogSensors; i++)
    {
        analogSensors[i].setPin(analogMuxPin);
        analogSensors[i].setHighLimit(100.0);
        analogSensors[i].setLowLimit(0.0);
    }
}

void analogSensorsRead()
{
    for (int i = 0; i < nAnalogSensors; i++)
    {
        analogMux.channel(i);
        analogSensors[i].read();
    }
}
