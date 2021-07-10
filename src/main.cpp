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

// timer clock

//long currentMillis = 0;
//long previousMillis = 0;
//long flowCalcMillis = 200;

// bool opn = true;
// bool cls = false;
//int address = 0x20;

// Обработка прерываний по входам с расходомеров

// void i2c_write(uint16_t data)
// {
//   Wire.beginTransmission(address);
//   Wire.write(lowByte(data));
//   Wire.write(highByte(data));
//   Wire.endTransmission();
// }

// Первоначальная настройка
void setup()
{
    Serial.begin(115200);
    // nullifyBuffers();
    // analogSensorsSetup();
    clockSetup();
    mbSetup();

    m1Setup();
    g1Setup();

    webSetup();

    // preferences.begin("", false);

    delay(1000);
}

//int counter = 0;
// RisingEdge t1pulse;
// FallingEdge t2pulse;

// Цикл
void loop()
{
    if (clk._1s)
    {
        // webPrintIp();

        // Serial.print("m1 = ");
        // Serial.print(m1.getFlow());

        // Serial.print("\tg1 = ");
        // Serial.println(dispenserCollector.flowmeter.getFlow());

        // incTimeouts();
        // plantLoop();

        // webLoop();

        // Serial.print("reg kl set = ");
        // Serial.println(valveAdjustable.getSetpoint());
        // Serial.print("reg kl pos = ");
        // Serial.print(valveAdjustable.getPosition());

        // Serial.print("kl 0 set = ");
        // Serial.println(dispenserCollector.valveAdjustable.getSetpoint());
        // Serial.print("\tkl 0 pos = ");
        // Serial.println(dispenserCollector.valveAdjustable.getPosition());

        // Serial.println(valveAdjustable.getStatus());
        // Serial.print("command open = ");
        // Serial.println(valveAdjustable.isCommandOpen());
        // Serial.print("command close = ");
        // Serial.println(valveAdjustable.isCommandClose());

        // Serial.println();

        // for (int i = 0; i < nAnalogSensors; i++)
        // {
        //   Serial.print(i);
        //   Serial.print('\t');
        //   Serial.println(analogSensors[i].getValue());
        // }
        // Serial.println();
    }

    if (clk._0_2s)
    {
        plantLoop();
        webLoop();
    }

    if (clk._0_1s)
    {
        // todo : разделить по времени, иначе ересь всякая лезет
        mbPoll();
        // mbReadAnalog();
        // mbWriteDiscrete();
        //mbWriteDiscrete21();
    }

    // loopPlant();
    clockReset();
}

// ==============================================================
// ==============================================================
// ==============================================================
// ==============================================================
// ==============================================================
// ==============================================================
// ==============================================================

//if (t1pulse.posEdge(t1.status))
// if (t1.status)
// {
//   Serial.print(millis());
//   Serial.println("\t400 ms");
// }

// if (t2pulse.falling(t2.status))
// {
//   Serial.print(millis());
//   Serial.println("\t600 ms");
//   //Serial.println(manifold.getFlowmeter()->getPin());
//   Serial.println(manifold.flowmeter.getPin());
// }
// //}

// t1.onTimer(!t2.status, 400);
// t2.offTimer(t1.status, 600);

//m1.calcVolume();

//currentMillis = millis();
//long intervalMillis = currentMillis - previousMillis;

//if (intervalMillis > flowCalcMillis)
//{
//flowCalc(&m1, intervalMillis);
//flowCalc(&g1, intervalMillis);
//}
// if ((currentMillis - startTime1 > 10000) && !timer1)
// {
//   timer1 = true;
//   startTime1 = currentMillis;
//   startTime2 = currentMillis;

//   set_pause = true;
//   set_open = false;
//   set_close = false;

// Serial.print("Opn: ");
// Serial.print(set_open, DEC);
// Serial.print("\tCls: ");
// Serial.print(set_close, DEC);
// Serial.print("\tPau: ");
// Serial.println(set_pause, DEC);

// if (set_pause)
// {
//   i2c_write(word(B11111111, B11111111));

//i2cHub2.digitalWrite(P8, HIGH);
//i2cHub2.digitalWrite(P9, HIGH);
//i2cHub2.digitalWrite(P10, HIGH);
//i2cHub2.digitalWrite(P11, HIGH);
//   }
// }

// if ((currentMillis - startTime2 > 2000) && timer1)
// {
//   timer1 = false;
//   startTime1 = currentMillis;
//   startTime2 = currentMillis;

//   set_pause = false;
//   to_open = !to_open;
//   set_open = to_open;
//   set_close = !to_open;

// Serial.print("Opn: ");
// Serial.print(set_open, DEC);
// Serial.print("\tCls: ");
// Serial.print(set_close, DEC);
// Serial.print("\tPau: ");
// Serial.println(set_pause, DEC);

// if (clk._10s)
// {
//   opn = !opn;
//   cls = !cls;
// }

// if (clk._10s && opn)
// {
//   Serial.println("Open");

//   for (int i = 0; i < 1; i += 2)
//   {
//     i2cHub1.digitalWrite(i, HIGH);
//     //i2cHub2.digitalWrite(i, HIGH);
//   }

//   for (int i = 1; i < 2; i += 2)
//   {
//     i2cHub1.digitalWrite(i, LOW);
//     //i2cHub2.digitalWrite(i, LOW);
//   }
// }

// if (clk._10s && cls)
// {
//   Serial.println("Close");

//   for (int i = 0; i < 1; i += 2)
//   {
//     i2cHub1.digitalWrite(i, LOW);
//     //i2cHub2.digitalWrite(i, LOW);
//   }

//   for (int i = 1; i < 2; i += 2)
//   {
//     i2cHub1.digitalWrite(i, HIGH);
//     //i2cHub2.digitalWrite(i, HIGH);
//   }
// }
// //}

//  clockReset();
//}