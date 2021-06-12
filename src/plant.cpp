#include "plant.h"
#include "clock_pulses.h"
#include <ProcessControl.h>

DispenserCollector dispenserCollector;
Flowmeter m1;
ValveAdjustable valveAdjustable;

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
    m1.setPulsesPerLiter(31.7);
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