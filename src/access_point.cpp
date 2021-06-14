#include "access_point.h"

float temperature, humidity, pressure, altitude;

/*Put your SSID & Password*/
const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

AsyncWebServer server(80);

String readValveAdjustablePosition()
{
    return String(valveAdjustable.getPosition());
}

String readDispenserValveAdjustablePosition()
{
    return String(dispenserCollector.valveAdjustable.getPosition());
}

String readAnalogSensorRaw0()
{
    return String(analogSensors[0].getRaw());
}

String readAnalogSensorRaw1()
{
    return String(analogSensors[1].getRaw());
}

bool parseJson(String jsonMessage)
{
    StaticJsonDocument<256> doc; //Memory pool
    auto error = deserializeJson(doc, jsonMessage);
    if (error)
    {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return false;
    }
    return true;
}

void webSetup()
{
    Serial.println();

    // Setting the ESP as an access point
    Serial.print("Setting AP (Access Point)…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // valveAdjustable
    server.on(
        "/valveAdjustable/position", HTTP_GET, [](AsyncWebServerRequest *request)
        { request->send_P(200, "text/plain", readValveAdjustablePosition().c_str()); });
    server.on(
        "/valveAdjustable/raw", HTTP_GET, [](AsyncWebServerRequest *request)
        { request->send_P(200, "text/plain", readAnalogSensorRaw0().c_str()); });
    server.on(
        "/valveAdjustable/fullyOpen", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            valveAdjustable.fullyOpen();
            request->send_P(200, "text/plain", "");
        });
    server.on(
        "/valveAdjustable/fullyClose", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            valveAdjustable.fullyClose();
            request->send_P(200, "text/plain", "");
        });

    // dispenserCollector valveAdjustable
    server.on(
        "/dispenserCollector/valveAdjustable/position", HTTP_GET, [](AsyncWebServerRequest *request)
        { request->send_P(200, "text/plain", readDispenserValveAdjustablePosition().c_str()); });
    server.on(
        "/dispenserCollector/valveAdjustable/raw", HTTP_GET, [](AsyncWebServerRequest *request)
        { request->send_P(200, "text/plain", readAnalogSensorRaw1().c_str()); });
    server.on(
        "/dispenserCollector/valveAdjustable/fullyOpen", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            dispenserCollector.valveAdjustable.fullyOpen();
            request->send_P(200, "text/plain", "");
        });
    server.on(
        "/dispenserCollector/valveAdjustable/fullyClose", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            dispenserCollector.valveAdjustable.fullyClose();
            request->send_P(200, "text/plain", "");
        });

    // dispenserCollector valves

    server.on(
        "/dispenserCollector/valves/0/open", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            dispenserCollector.valves[0].open();
            request->send_P(200, "text/plain", "");
        });
    server.on(
        "/dispenserCollector/valves/0/close", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            dispenserCollector.valves[0].close();
            request->send_P(200, "text/plain", "");
        });

    server.on(
        "/dispenserCollector/valves/1/open", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            dispenserCollector.valves[1].open();
            request->send_P(200, "text/plain", "");
        });
    server.on(
        "/dispenserCollector/valves/1/close", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            dispenserCollector.valves[1].close();
            request->send_P(200, "text/plain", "");
        });

    server.on(
        "/dispenserCollector/valves/2/open", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            dispenserCollector.valves[2].open();
            request->send_P(200, "text/plain", "");
        });
    server.on(
        "/dispenserCollector/valves/2/close", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            dispenserCollector.valves[2].close();
            request->send_P(200, "text/plain", "");
        });

    server.on(
        "/dispenserCollector/valves/3/open", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            dispenserCollector.valves[3].open();
            request->send_P(200, "text/plain", "");
        });
    server.on(
        "/dispenserCollector/valves/3/close", HTTP_GET, [](AsyncWebServerRequest *request)
        {
            dispenserCollector.valves[3].close();
            request->send_P(200, "text/plain", "");
        });
    // server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send_P(200, "text/plain", readHumi().c_str()); });
    // server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send_P(200, "text/plain", readPres().c_str()); });

    // Example of function that holds and HTTP_POST request on 192.168.4.1:80/set_data
    // server.on(
    //     "/valveAdjustable/setpoint", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
    //     [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
    //     {
    //         // Here goes the code to manage the post request
    //         // The data is received on 'data' variable
    //         // Parse data
    //         Serial.println("POST RECEIVED"); // Just for debug
    //         // Serial.print("len = ");          // Just for debug
    //         // Serial.println(len);             // Just for debug
    //         Serial.print("data = "); // Just for debug
    //         Serial.println(data);    // Just for debug

    //         // StaticJsonBuffer<50> JSONBuffer;                   // create a buffer that fits for you
    //         // JsonObject &parsed = JSONBuffer.parseObject(data); //Parse message
    //         // uint8_t received_data = parsed["setpoint"];        //Get data
    //         // Serial.println(received_data);

    //         // parseJson(data);

    //         request->send(200, "text/plain", "setpoint");
    //     });

    // Start server
    server.begin();
}

void webLoop()
{
    // char JSONMessage[] = "{\"SensorType\": \"Temperature\", \"Value\": 10}"; //Original message
}