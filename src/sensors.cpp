#include "sensors.h"
#include "plant.h"

// Analog inputs
//const int inPosManifold0 = 0;
//const int inPosValve1 = 1;
//const int inPosValve2 = 2;
//const int inPosValve3 = 3;
//const int inPosValve4 = 4;

//const int inPosValve4 = 3;

//const int analogMuxS0 = 26;
//const int analogMuxS1 = 25;
//const int analogMuxS2 = 33;
//const int analogMuxS3 = 32;
//const int analogMuxPin = 35;

// multiplexer
//CD74HC4067 analogMux(analogMuxS0, analogMuxS1, analogMuxS2, analogMuxS3);

// sensors array
const int nAnalogSensors = 6;
AnalogSensor analogSensors[nAnalogSensors];

void analogSensorsSetup()
{
    for (int i = 0; i < nAnalogSensors; i++)
    {
        analogSensors[i].setHighLimit(100.0);
        analogSensors[i].setLowLimit(0.0);
    }
}

void analogSensorsRead()
{
    valveControl.setPosition(analogSensors[0].getValue());
    manifold.valves[0].setPosition(analogSensors[1].getValue());
    manifold.valves[1].setPosition(analogSensors[2].getValue());
    manifold.valves[2].setPosition(analogSensors[3].getValue());
    manifold.valves[3].setPosition(analogSensors[4].getValue());
    manifold.valves[4].setPosition(analogSensors[5].getValue());
}
