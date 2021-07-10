#ifndef ACCESS_POINT_h
#define ACCESS_POINT_h

#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"

#include "plant.h"
#include "sensors.h"
#include "clock_pulses.h"
#include "json_serialize.h"
#include "json_deserialize.h"

// extern const char *ssid;
// extern const char *password;

// Set web server port number to 80
// extern WebServer server;

// Variable to store the HTTP request
// extern String header;

void webSetup();
void webLoop();

#endif