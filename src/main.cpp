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

    // dataSetup();
    webSetup();
    delay(1000);

    clockSetup();
    mbSetup();
    plantSetup();

    delay(3000);
}

// Цикл
void loop()
{
    // long loop_start = micros();

    mbPoll();
    plantLoop();

    if (clk._10ms)
    {
        // mbPoll();
        // plantLoop();
        // Serial.println("clock");
        // опрос устройств Modbus
        // mbPoll();

        // выполняем действия с управлением установкой
        // plantLoop();
    }
    // if (clk._50ms)
    // {
    // Serial.println("clock 50 ms");
    // опрос устройств Modbus
    // mbPoll();

    // выполняем действия с управлением установкой
    // plantLoop();
    // }
    if (clk._100ms)
    {
        // опрос устройств Modbus
        // mbPoll();

        // выполняем действия с управлением установкой
        // plantLoop();
    }

    if (clk._100ms)
    {
        // увеличиваем таймеры для аварий по отсутствию движения
        incTimeouts();
        // увеличиваем разные таймеры коллектора
        incTimers();
        // выполняем действия с управлением установкой
        // plantLoop();
    }
    if (clk._200ms)
    {
        // // выполняем действия с управлением установкой
        // plantLoop();
        // обмен по Wi-Fi
        webLoop();
    }
    if (clk._500ms)
    {
        // считаем текущий расход
        flowLoop();

        // Serial.print("\tlDon: ");
        // Serial.print(collector.loopDone_);

        Serial.print("\tlReq: ");
        for (int i = 0; i < collector.nValves_ - 1; i++)
        {
            Serial.print(collector.requiredVolumes[i]);
            Serial.print(", ");
        }

        Serial.print("\tlDos: ");
        for (int i = 0; i < collector.nValves_ - 1; i++)
        {
            Serial.print(collector.dosedVolumes[i]);
            Serial.print(", ");
        }

        Serial.print("\tlOffset: ");
        Serial.print(collector.dosingVolumeOffset_);
        Serial.println();

        // Serial.print("\tdose: ");
        // Serial.print(collector.dosing_);
        // Serial.print("\tdFin: ");
        // Serial.print(collector.dosingFinishing_);
        // Serial.print("\tdDel: ");
        // Serial.print(collector.dosingDoneDelay_);
        // Serial.print("\tdDon: ");
        // Serial.print(collector.dosingDone_);

        // Serial.print("\twash: ");
        // Serial.print(collector.washing_);
        // Serial.print("\twFin: ");
        // Serial.print(collector.washingFinishing_);
        // Serial.print("\twDon: ");
        // Serial.println(collector.washingDone_);
    }
    // if (clk._1s)
    // {
    //     // todo: здесь то, что выполняем раз в секунду
    // }

    // mixLoop();
    clockReset();

    // long loop_time = micros() - loop_start;
    // Serial.println(loop_time);
}
