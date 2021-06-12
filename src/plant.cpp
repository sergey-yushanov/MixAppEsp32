#include "plant.h"
#include "clock_pulses.h"
#include <ProcessControl.h>

Manifold manifold;
Flowmeter m1;
Valve valveControl;

bool ack;

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

//
void plantProcess()
{
    // РегКл
    //valveControl.process();

    // пуск насоса
    // ...

    // открываем РегКл
    // onTimerOpen.onTimer(!valveControl.isOpen() && !onTimerClose.status, 10000);
    // if (onTimerOpen.status)
    // {
    // valveControl.open();
    // Serial.println("РегКл - открываем");
    // }

    // закрываем РегКл
    // onTimerClose.onTimer(!valveControl.isClose() && !onTimerOpen.status, 10000);
    // if (onTimerClose.status)
    // {
    // if (valveControl.isOpen())
    // {
    //     valveControl.close();
    // }

    // if (valveControl.isClose())
    // {
    //     valveControl.open();
    // }
    //Serial.println("РегКл - закрываем");
    // }

    //valveControl.setSetpoint(34.0);
    // valveControl.process(clk._0_1s, true);
}