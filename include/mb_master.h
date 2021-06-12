#ifndef MB_MASTER_h
#define MB_MASTER_h

#include "ModbusClientRTU.h"

extern ModbusClientRTU MB;
extern uint32_t mbToken;

extern uint8_t buffer20[];
extern uint8_t buffer21[];

//void handleData(ModbusMessage response, uint32_t token);
//void handleError(Error error, uint32_t token);

void nullifyBuffers();
void mbSetup();
void mbReadAnalog();
void mbSetDiscrete();
void mbWriteDiscrete();
//void mbWriteDiscrete21();

#endif