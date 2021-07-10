#include "plant.h"
#include "clock_pulses.h"
#include <ProcessControl.h>

DispenserCollector dispenserCollector;
Flowmeter m1;
ValveAdjustable valveAdjustable;

bool ack;

// dispenser collector flowmeter
void g1Setup()
{
    dispenserCollector.flowmeter.setPin(19);
    dispenserCollector.flowmeter.setPulsesPerLiter(106.777);
    pinMode(dispenserCollector.flowmeter.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(dispenserCollector.flowmeter.getPin()), g1Pulse, FALLING);
}

void IRAM_ATTR g1Pulse()
{
    dispenserCollector.flowmeter.pulseCounter();
}

// common flowmeter
void m1Setup()
{
    m1.setPin(18);
    m1.setPulsesPerLiter(31.7); // 50.0
    pinMode(m1.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m1.getPin()), m1Pulse, FALLING);
}

void IRAM_ATTR m1Pulse()
{
    m1.pulseCounter();
}

// timeouts
void incTimeouts()
{
    valveAdjustable.incTimeout();
    dispenserCollector.valveAdjustable.incTimeout();
}

// reset faults for all equipment
void resetFaults()
{
    valveAdjustable.resetFaulty();
    dispenserCollector.valveAdjustable.resetFaulty();
    for (int i = 0; i < dispenserCollector.nValves_; i++)
    {
        dispenserCollector.valves[i].resetFaulty();
    }
}

void plantLoop()
{
    incTimeouts();

    if (ack)
    {
        resetFaults();
        ack = false;
    }
}
