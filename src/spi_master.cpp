#include "spi_master.h"
#include "plant.h"
#include "sensors.h"

const int chipSelectDrv1 = 32;
const int chipSelectDrv2 = 33;

const int chipSelectAdc1 = 22;
const int chipSelectAdc2 = 25;

DRV89xxDevice motorDevice1(chipSelectDrv1, 0, 0);
DRV89xxDevice motorDevice2(chipSelectDrv2, 0, 0);

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

    motorDevice1.begin();
    motorDevice2.begin();
    delay(100);
}

// устанавливаем значения для записи по Modbus
void spiWriteDiscrete()
{
    motorDevice1.writeBit(DRV89xx_OP_CTRL_2, 7, pumpCommand);

    motorDevice2.writeBit(DRV89xx_OP_CTRL_1, 1, collector.valves[2].isCommand());
    motorDevice2.writeBit(DRV89xx_OP_CTRL_1, 3, collector.valves[1].isCommand());
    motorDevice2.writeBit(DRV89xx_OP_CTRL_1, 7, collector.valveAdjustable.isCommandClose());

    motorDevice2.writeBit(DRV89xx_OP_CTRL_2, 1, collector.valves[3].isCommand());
    motorDevice2.writeBit(DRV89xx_OP_CTRL_2, 3, collector.valveAdjustable.isCommandOpen());
    motorDevice2.writeBit(DRV89xx_OP_CTRL_2, 5, singleDos.valveAdjustable.isCommandOpen());
    motorDevice2.writeBit(DRV89xx_OP_CTRL_2, 7, collector.valves[0].isCommand());

    motorDevice2.writeBit(DRV89xx_OP_CTRL_3, 1, singleDos.valveAdjustable.isCommandClose());

    motorDevice1.writeRegisters();
    motorDevice2.writeRegisters();
}

uint16_t readIntRaw(float mV, uint16_t offset_uA)
{
    float mA = mV / 120.0;
    float uA = mA * 1000.0;

    return (uint16_t)round(uA) - offset_uA;
}

void spiReadAnalog()
{
    uint16_t value;

    SPI.beginTransaction(adc2.get_spi_settings());
    if (adc2.update())
    {
        value = readIntRaw(adc2.readmV(0), 0);
        collector.valveAdjustable.getPositionSensor()->setIntRaw(value);
        collector.valveAdjustable.updatePosition();

        value = readIntRaw(adc2.readmV(2), 0);
        singleDos.valveAdjustable.getPositionSensor()->setIntRaw(value);
        singleDos.valveAdjustable.updatePosition();
    }
    SPI.endTransaction();
}

void spiPoll()
{
    digitalWrite(chipSelectAdc1, HIGH);
    digitalWrite(chipSelectAdc2, HIGH);

    spiWriteDiscrete();

    digitalWrite(chipSelectDrv1, HIGH);
    digitalWrite(chipSelectDrv2, HIGH);

    spiReadAnalog();
}