#include "plant.h"
#include "clock_pulses.h"
#include <ProcessControl.h>

Collector collector;
Flowmeter m1;
ValveAdjustable valveAdjustable;

bool ack;
bool showSettings;
bool pumpCommand;

// dispenser collector flowmeter
void g1Setup()
{
    collector.flowmeter.setPin(19);
    // dispenserCollector.flowmeter.setPulsesPerLiter(106.777);
    pinMode(collector.flowmeter.getPin(), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(collector.flowmeter.getPin()), g1Pulse, FALLING);
}

void IRAM_ATTR g1Pulse()
{
    collector.flowmeter.pulseCounter();
}

// common flowmeter
void m1Setup()
{
    m1.setPin(18);
    // m1.setPulsesPerLiter(31.7); // 50.0
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
    collector.valveAdjustable.incTimeout();
}

// reset faults for all equipment
void resetFaults()
{
    valveAdjustable.resetFaulty();
    collector.valveAdjustable.resetFaulty();
    for (int i = 0; i < collector.nValves_; i++)
    {
        collector.valves[i].resetFaulty();
    }
}

void plantSetup()
{
    ack = true;
    showSettings = true;

    dataSetup();

    m1Setup();
    g1Setup();
}

void plantLoop()
{
    if (ack)
    {
        resetFaults();
        ack = false;
    }
}
