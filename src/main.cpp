#include <Arduino.h>

#include "clock_pulses.h"
#include "plant.h"
#include "mb_master.h"
#include "access_point.h"
#include "save_data.h"

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
    dataSetup();

    webSetup();

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
        mbPoll();
        // увеличиваем таймеры для аварий по отсутствию движения
        incTimeouts();
    }

    clockReset();
}
