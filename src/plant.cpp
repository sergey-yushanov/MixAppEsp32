#include "plant.h"

Manifold manifold;
Flowmeter m1;

// manifold flowmeter
void g1Setup()
{
    manifold.flowmeter.setPin(19);
    manifold.flowmeter.setPulsesPerLiter(106.777);
    pinMode(manifold.flowmeter.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(manifold.flowmeter.getPin()), g1Pulse, FALLING);
}

void IRAM_ATTR g1Pulse()
{
    manifold.flowmeter.pulseCounter();
}

// common flowmeter
void m1Setup()
{
    m1.setPin(18);
    m1.setPulsesPerLiter(31.7);
    pinMode(m1.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m1.getPin()), m1Pulse, FALLING);
}

void IRAM_ATTR m1Pulse()
{
    m1.pulseCounter();
}