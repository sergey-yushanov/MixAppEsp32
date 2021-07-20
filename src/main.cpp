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

    clockSetup();
    mbSetup();
    plantSetup();
    // dataSetup();
    webSetup();

    delay(1000);
}

// Цикл
void loop()
{
    if (clk._0_1s)
    {
        // опрос устройств Modbus
        mbPoll();
        // увеличиваем таймеры для аварий по отсутствию движения
        incTimeouts();
    }
    if (clk._0_2s)
    {
        // выполняем действия с управлением установкой
        plantLoop();
        // обмен по Wi-Fi
        webLoop();
    }
    if (clk._0_5s)
    {
        // считаем текущий расход
        flowLoop();
    }
    if (clk._1s)
    {
        // todo: здесь то, что выполняем раз в секунду
    }

    mixLoop();
    clockReset();
}
