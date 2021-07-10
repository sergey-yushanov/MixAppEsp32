#include <Arduino.h>

#include <Preferences.h>

//#include <PCF8575.h>
#include "sensors.h"
#include "clock_pulses.h"
#include "plant.h"
//#include "i2c_hub.h"
#include "mb_master.h"

#include "access_point.h"

Preferences preferences;

// Первоначальная настройка
void setup()
{
    Serial.begin(115200);
    // nullifyBuffers();
    // analogSensorsSetup();
    clockSetup();
    mbSetup();

    // m1Setup();
    // g1Setup();
    plantSetup();

    webSetup();

    // preferences.begin("", false);

    delay(1000);
}

// Цикл
void loop()
{
    if (clk._1s)
    {
        // todo: здесь то, что выполняем раз в секунду
    }

    if (clk._0_2s)
    {
        plantLoop();
        webLoop();
    }

    if (clk._0_1s)
    {
        // todo: разделить по времени, иначе ересь всякая лезет
        mbPoll();
        incTimeouts();
    }

    clockReset();
}
