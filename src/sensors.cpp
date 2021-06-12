#include "sensors.h"
#include "plant.h"

// sensors array
const int nAnalogSensors = 6;
AnalogSensor analogSensors[nAnalogSensors];

void analogSensorsSetup()
{
    // todo: load sensors limits from preferencies
}

void analogSensorsRead()
{
    valveAdjustable.setPosition(analogSensors[0].getValue());
    dispenserCollector.valveAdjustable.setPosition(analogSensors[1].getValue());

    // valveControl.setPosition(analogSensors[0].getValue());
    // manifold.valves[0].setPosition(analogSensors[1].getValue());
    // manifold.valves[1].setPosition(analogSensors[2].getValue());
    // manifold.valves[2].setPosition(analogSensors[3].getValue());
    // manifold.valves[3].setPosition(analogSensors[4].getValue());
    // manifold.valves[4].setPosition(analogSensors[5].getValue());
}
