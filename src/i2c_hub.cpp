#include "i2c_hub.h"

// Set i2c address
PCF8575 i2cHub1(0x20, I2C_SDA, I2C_SCL);
PCF8575 i2cHub2(0x21, I2C_SDA, I2C_SCL);

void i2cSetup()
{
    for (int i = 0; i <= 15; i++)
    {
        i2cHub1.pinMode(i, OUTPUT);
    }

    for (int i = 0; i <= 15; i++)
    {
        i2cHub2.pinMode(i, OUTPUT);
    }

    i2cHub1.begin();
    i2cHub1.digitalWriteAll(word(B11111111, B11111111));
    i2cHub2.begin();
    i2cHub2.digitalWriteAll(word(B11111111, B11111111));
}

void i2cWrite()
{
}
