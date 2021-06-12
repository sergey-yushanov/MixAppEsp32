#include "mb_master.h"
#include "plant.h"
#include "sensors.h"

ModbusClientRTU MB(Serial2);
uint32_t mbToken = 1111;

uint8_t buffer20[24];
uint8_t buffer21[24];

void nullifyBuffers()
{
    for (int i = 0; i < 24; i++)
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
        analogSensors[0].setValueRaw(value);
        response.get(3 + 1 * 2, value);
        analogSensors[1].setValueRaw(value);
        response.get(3 + 2 * 2, value);
        analogSensors[2].setValueRaw(value);
        response.get(3 + 3 * 2, value);
        analogSensors[3].setValueRaw(value);
        response.get(3 + 4 * 2, value);
        analogSensors[4].setValueRaw(value);
        response.get(3 + 5 * 2, value);
        analogSensors[5].setValueRaw(value);

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
        //Serial.print("Error creating request: ");
        Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        //LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
    }
}

// устанавливаем значения для записи по Modbus
void mbSetDiscrete()
{
    buffer20[0] = valveControl.commandOpen ? 1 : 0;
    buffer20[1] = valveControl.commandClose ? 1 : 0;

    // for (auto &byte : buffer20)
    // {
    //     Serial.printf("%02X ", byte);
    // }
    // Serial.println();
}

void mbWriteDiscrete() //(uint8_t *buffer)
{
    mbSetDiscrete();

    Error err = MB.addRequest(mbToken++, 20, WRITE_MULT_COILS, 0, 12, 2, buffer20);
    if (err != SUCCESS)
    {
        ModbusError e(err);
        //Serial.print("Error creating request: ");
        Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        //LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
    }

    err = MB.addRequest(mbToken++, 21, WRITE_MULT_COILS, 0, 12, 2, buffer21);
    if (err != SUCCESS)
    {
        ModbusError e(err);
        //Serial.print("Error creating request: ");
        Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        //LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
    }
}