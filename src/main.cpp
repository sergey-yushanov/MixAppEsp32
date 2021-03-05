#include <Arduino.h>

//#include <Analog.h>
//#include <PCA.h>

//#include <ProcessControl.h>

//#include <Flowmeter.h>

//#include <Manifold.h>
#include <PCF8575.h>
//#include <CD74HC4067.h>
//#include <Wire.h>

#include "sensors.h"
#include "clock_pulses.h"
#include "plant.h"
#include "i2c_hub.h"

//int delayTime = 1000;

//OnTimer t1;
//OffTimer t2;

//Manifold manifolds[1];

// расходомеры с импульсными выходами

//arrayCollector[1];
//arrayValve[4];

// flowmeters pulses
// Flowmeter m1(14, 31.7);
// void IRAM_ATTR m1Pulse()
// {
//   m1.pulseCounter();
// }

// timer clock

//long currentMillis = 0;
//long previousMillis = 0;
//long flowCalcMillis = 200;

bool opn = true;
bool cls = false;
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
  //pinMode(sigPin, INPUT);

  i2cSetup();
  //m1Setup();
  //plantManifoldSetup();

  analogSensorsSetup();
  Serial.begin(115200);
  opn = true;
  cls = false;
  clockSetup();

  delay(1000);
}

//int counter = 0;
// RisingEdge t1pulse;
// FallingEdge t2pulse;

// Цикл
void loop()
{
  analogSensorsRead();

  // if (clk._0_1s)
  // {
  //   for (int i = 0; i < nAnalogSensors; i++)
  //   {
  //     Serial.print("Channel ");
  //     Serial.print(i);
  //     Serial.print(" = ");
  //     Serial.println(analogSensors[i].getValue());
  //   }
  // }

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

  if (clk._10s)
  {
    opn = !opn;
    cls = !cls;
  }

  if (clk._10s && opn)
  {
    Serial.println("Open");

    for (int i = 0; i < 1; i += 2)
    {
      i2cHub1.digitalWrite(i, HIGH);
      //i2cHub2.digitalWrite(i, HIGH);
    }

    for (int i = 1; i < 2; i += 2)
    {
      i2cHub1.digitalWrite(i, LOW);
      //i2cHub2.digitalWrite(i, LOW);
    }
  }

  if (clk._10s && cls)
  {
    Serial.println("Close");

    for (int i = 0; i < 1; i += 2)
    {
      i2cHub1.digitalWrite(i, LOW);
      //i2cHub2.digitalWrite(i, LOW);
    }

    for (int i = 1; i < 2; i += 2)
    {
      i2cHub1.digitalWrite(i, HIGH);
      //i2cHub2.digitalWrite(i, HIGH);
    }
  }
  //}

  clockReset();
}