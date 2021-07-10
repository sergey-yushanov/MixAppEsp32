#ifndef JSON_SERIALIZE_h
#define JSON_SERIALIZE_h

#include "ArduinoJson.h"
#include <DispenserCollector.h>
#include <Flowmeter.h>
#include <AnalogSensor.h>
#include <Valve.h>
#include <ValveAdjustable.h>

String jsonSerialize(ValveAdjustable, Flowmeter, DispenserCollector, bool);

#endif