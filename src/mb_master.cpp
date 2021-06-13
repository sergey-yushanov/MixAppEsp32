#include "mb_master.h"
#include "plant.h"
#include "sensors.h"

ModbusClientRTU MB(Serial2);
uint32_t mbToken = 1111;

int addrCounter = 0;

uint8_t buffer20[2];
uint8_t buffer21[2];

void nullifyBuffers()
{
    for (int i = 0; i < 2; i++)
    {
        buffer20[i] = 0;
        buffer21[i] = 0;
    }
}

// Define an onData handler function to receive the regular responses
// Arguments are Modbus server ID, the function code requested, the message data and length of it,
// plus a user-supplied token to identify the causing request
void handleData(ModbusMessage response, uint32_t token)
{
    // for (auto &byte : response)
    // {
    //     Serial.printf("%02X ", byte);
    // }
    // Serial.println();

    if (response.getServerID() == 10)
    {
        uint16_t value;
        response.get(3 + 0 * 2, value);
        analogSensors[0].setIntRaw(value);
        response.get(3 + 1 * 2, value);
        analogSensors[1].setIntRaw(value);
        response.get(3 + 2 * 2, value);
        analogSensors[2].setIntRaw(value);
        response.get(3 + 3 * 2, value);
        analogSensors[3].setIntRaw(value);
        response.get(3 + 4 * 2, value);
        analogSensors[4].setIntRaw(value);
        response.get(3 + 5 * 2, value);
        analogSensors[5].setIntRaw(value);

        analogSensorsRead();
    }
}

// Define an onError handler function to receive error responses
// Arguments are the error code returned and a user-supplied token to identify the causing request
void handleError(Error error, uint32_t token)
{
    // ModbusError wraps the error code and provides a readable error message for it
    ModbusError me(error);
    Serial.printf("Error creating request: %02X - %s\n", (int)me, (const char *)me);
}

void mbSetup()
{
    nullifyBuffers();
    Serial2.begin(9600, SERIAL_8N1);
    MB.onDataHandler(&handleData);
    MB.onErrorHandler(&handleError);
    MB.setTimeout(2000);
    MB.begin();
}

void mbReadAnalog()
{
    Error err = MB.addRequest(mbToken++, 10, READ_HOLD_REGISTER, 0, 6);
    if (err != SUCCESS)
    {
        ModbusError e(err);
        Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        //LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
    }
}

// устанавливаем значение бита в байте
uint8_t setBufferBit(uint8_t number, int xBit, int nBit)
{
    number ^= (-xBit ^ number) & (1UL << nBit);
    return number;
}

// устанавливаем значения для записи по Modbus
void mbSetDiscrete()
{
    buffer20[0] = setBufferBit(buffer20[0], valveAdjustable.isCommandOpen(), 0);
    buffer20[0] = setBufferBit(buffer20[0], valveAdjustable.isCommandClose(), 1);
    buffer20[0] = setBufferBit(buffer20[0], dispenserCollector.valveAdjustable.isCommandOpen(), 2);
    buffer20[0] = setBufferBit(buffer20[0], dispenserCollector.valveAdjustable.isCommandClose(), 3);
    buffer20[0] = setBufferBit(buffer20[0], dispenserCollector.valves[0].isCommandOpen(), 4);
    buffer20[0] = setBufferBit(buffer20[0], dispenserCollector.valves[0].isCommandClose(), 5);
    buffer20[0] = setBufferBit(buffer20[0], dispenserCollector.valves[1].isCommandOpen(), 6);
    buffer20[0] = setBufferBit(buffer20[0], dispenserCollector.valves[1].isCommandClose(), 7);

    buffer20[1] = setBufferBit(buffer20[1], dispenserCollector.valves[2].isCommandOpen(), 0);
    buffer20[1] = setBufferBit(buffer20[1], dispenserCollector.valves[2].isCommandClose(), 1);

    buffer21[0] = setBufferBit(buffer21[0], dispenserCollector.valves[3].isCommandOpen(), 0);
    buffer21[0] = setBufferBit(buffer21[0], dispenserCollector.valves[3].isCommandClose(), 3);

    // for (auto &byte : buffer20)
    // {
    //     Serial.printf("%02X ", byte);
    // }
    // Serial.println();
}

// void mbWriteDiscrete(uint8_t serverID, uint16_t p1, uint16_t p2, uint8_t count, uint8_t *buffer)
// {
//     // mbSetDiscrete();

//     Error err = MB.addRequest(mbToken++, 20, WRITE_MULT_COILS, 0, 12, 2, buffer20);
//     if (err != SUCCESS)
//     {
//         ModbusError e(err);
//         Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
//         //LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
//     }

//     err = MB.addRequest(mbToken++, 21, WRITE_MULT_COILS, 0, 12, 2, buffer21);
//     if (err != SUCCESS)
//     {
//         ModbusError e(err);
//         Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
//         //LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
//     }
// }

void mbWriteDiscrete20() //(uint8_t *buffer)
{
    // mbSetDiscrete();

    Error err = MB.addRequest(mbToken++, 20, WRITE_MULT_COILS, 0, 12, 2, buffer20);
    if (err != SUCCESS)
    {
        ModbusError e(err);
        Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        //LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
    }
}

void mbWriteDiscrete21()
{
    // mbSetDiscrete();

    Error err = MB.addRequest(mbToken++, 21, WRITE_MULT_COILS, 0, 12, 2, buffer21);
    if (err != SUCCESS)
    {
        ModbusError e(err);
        Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        //LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
    }
}

// опрос всех адресов последовательно
void mbPoll()
{
    mbSetDiscrete();

    if (addrCounter == 0)
        mbReadAnalog();
    if (addrCounter == 1)
        mbWriteDiscrete20();
    if (addrCounter == 2)
        mbWriteDiscrete21();

    addrCounter++;
    if (addrCounter > 2)
        addrCounter = 0;
}