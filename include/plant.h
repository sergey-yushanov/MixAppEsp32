#ifndef PLANT_h
#define PLANT_h

#include <Manifold.h>
#include <Flowmeter.h>
//#include <PCF8575.h>

//extern PCF8575 i2cHub1; //, I2C_SDA, I2C_SCL);
//extern PCF8575 i2cHub2;

extern Manifold manifold;
extern Flowmeter m1;

//void i2cSetup();

void m1Setup();
void IRAM_ATTR m1Pulse();

void g1Setup();
void IRAM_ATTR g1Pulse();

#endif