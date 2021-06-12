#ifndef AP_h
#define AP_h

#include <WiFi.h>

extern const char *ssid;
extern const char *password;

// Set web server port number to 80
extern WiFiServer server;

// Variable to store the HTTP request
extern String header;

void clientSetup();
void clientLoop();

#endif