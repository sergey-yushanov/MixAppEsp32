#include "spi_master.h"
#include "plant.h"
#include "sensors.h"

const int chipSelectDrv1 = 32;
const int chipSelectDrv2 = 33;

const int chipSelectAdc1 = 22;
const int chipSelectAdc2 = 25;

DRV89xx motorDriver1(chipSelectDrv1, 0, 0);
DRV89xx motorDriver2(chipSelectDrv2, 0, 0);

// AD779X adc1(5.0);
AD779X adc2(3.3);

void spiSetup()
{
    SPI.begin();
    // SPI.setDataMode(SPI_MODE3); // datasheet p6-7
    // SPI.setBitOrder(MSBFIRST);
    // SPI.setFrequency(4000000);
    // SPI.setClockDivider(SPI_CLOCK_DIV32); // datasheet p6

    pinMode(chipSelectDrv1, OUTPUT);
    pinMode(chipSelectDrv2, OUTPUT);
    digitalWrite(chipSelectDrv1, HIGH);
    digitalWrite(chipSelectDrv2, HIGH);

    pinMode(chipSelectAdc1, OUTPUT);
    pinMode(chipSelectAdc2, OUTPUT);
    digitalWrite(chipSelectAdc1, HIGH);
    digitalWrite(chipSelectAdc2, HIGH);

    SPI.beginTransaction(adc2.get_spi_settings());
    adc2.begin(chipSelectAdc2); // ADC attached to CS pin 10
    adc2.setup();               // default values: 3 channels, 0...2
    adc2.config(0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00);
    SPI.endTransaction();
    delay(100);

    motorDriver1.begin();
    motorDriver2.begin();
    delay(100);
}

void spiPoll()
{
    digitalWrite(chipSelectAdc1, HIGH);
    digitalWrite(chipSelectAdc2, HIGH);
    SPI.beginTransaction(motorDriver1.get_spi_settings());

    for (int i = 1; i < 8; i += 2)
    {
        motorDriver1.writeRegister(0x08, 1 << i);
        delay(20);
        motorDriver1.writeRegister(0x08, 0 << i);
        delay(20);
    }

    for (int i = 1; i < 8; i += 2)
    {
        motorDriver1.writeRegister(0x09, 1 << i);
        delay(20);
        motorDriver1.writeRegister(0x09, 0);
        delay(20);
    }

    for (int i = 1; i < 4; i += 2)
    {
        motorDriver1.writeRegister(0x0A, 1 << i);
        delay(20);
        motorDriver1.writeRegister(0x0A, 0);
        delay(20);
    }

    for (int i = 1; i < 8; i += 2)
    {
        motorDriver2.writeRegister(0x08, 1 << i);
        delay(20);
        motorDriver2.writeRegister(0x08, 0);
        delay(20);
    }

    for (int i = 1; i < 8; i += 2)
    {
        motorDriver2.writeRegister(0x09, 1 << i);
        delay(20);
        motorDriver2.writeRegister(0x09, 0);
        delay(20);
    }

    for (int i = 1; i < 4; i += 2)
    {
        motorDriver2.writeRegister(0x0A, 1 << i);
        delay(20);
        motorDriver2.writeRegister(0x0A, 0);
        delay(20);
    }
    /**/
    SPI.endTransaction();

    digitalWrite(chipSelectDrv1, HIGH);
    digitalWrite(chipSelectDrv2, HIGH);
    // digitalWrite(chipSelectAdc1, LOW);
    // digitalWrite(chipSelectAdc2, LOW);
    SPI.beginTransaction(adc2.get_spi_settings());
    if (adc2.update())
    { // if new values available
        for (int i = 2; i < 3; i++)
        { // print RAW and mV values
            Serial.print("CH");
            Serial.print(i);
            Serial.print(" - RAW: ");
            Serial.print(adc2.readRaw(i), HEX);
            Serial.print("\tmV: ");
            Serial.println(adc2.readmV(i), HEX);
        }
    }
    SPI.endTransaction();
}