#ifndef JSON_SERIALIZE_h
#define JSON_SERIALIZE_h

#include "ArduinoJson.h"
#include <Collector.h>
#include <Flowmeter.h>
#include <AnalogSensor.h>
#include <Valve.h>
#include <ValveAdjustable.h>
#include "plant.h"

String jsonSerialize(ValveAdjustable, Flowmeter, Collector, SingleDos, bool);

#endif