#ifndef I2C_HUB_h
#define I2C_HUB_h

#include <PCF8575.h>

#define I2C_SDA 21
#define I2C_SCL 22

extern PCF8575 i2cHub1;
extern PCF8575 i2cHub2;

void i2cSetup();
void i2cWrite();

#endif