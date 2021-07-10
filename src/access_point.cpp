#include "access_point.h"

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

String jsonToSend;
bool showSettings = false;

// Allocate the received JSON
StaticJsonDocument<200> jsonReceived;

const char index_html[] PROGMEM = R"rawliteral(
    <html>
        <head>
            <script>
                var gateway = `ws://${window.location.hostname}/ws`;
                var websocket;
                window.addEventListener('load', onLoad);
                function initWebSocket() {
                    console.log('Trying to open a WebSocket connection...');
                    websocket = new WebSocket(gateway);
                    websocket.onopen    = onOpen;
                    websocket.onclose   = onClose;
                    websocket.onmessage = onMessage;
                    websocket.onerror   = onError;
                }
                function onOpen(event) {
                    console.log('Connection opened');
                }
                function onClose(event) {
                    console.log('Connection closed');
                    setTimeout(initWebSocket, 2000);
                }
                function onMessage(event) {
                    console.log(event.data);
                }
                function onError (event) {
                    console.log("ERROR: " + event.data);
                }
                function onLoad(event) {
                    initWebSocket();
                }
                function doSend(message) {
                    console.log("SENT: " + message);
                    websocket.send(message);
                }
            </script>
        </head>
    </html>
)rawliteral";

void notifyClients()
{
    ws.textAll(jsonToSend.c_str(), jsonToSend.length());
}

void messageContainsKeys()
{
    if (jsonReceived.containsKey("showSettings"))
    {
        showSettings = jsonReceived["showSettings"];
    }

    if (jsonReceived.containsKey("ack"))
    {
        ack = jsonReceived["ack"];
    }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        // Serial.println((char *)data);

        // Deserialize the JSON document
        DeserializationError error = deserializeJson(jsonReceived, data);
        // Test if parsing succeeds.
        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }
        // check message keys
        messageContainsKeys();
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void initWebSocket()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void webSetup()
{
    Serial.println();

    // Setting the ESP as an access point
    Serial.print("Setting AP (Access Point)…");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    Serial.println("Wait 100 ms for AP_START...");
    delay(100);

    Serial.println("Set softAPConfig");
    IPAddress Ip(192, 168, 11, 1);
    IPAddress NMask(255, 255, 255, 0);
    WiFi.softAPConfig(Ip, Ip, NMask);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    // Init WebSockets
    initWebSocket();

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html); });

    // Start server
    server.begin();

    showSettings = false;
}

String boolToString(bool value)
{
    return value ? "true" : "false";
}

String valveJSON(int number, Valve valve)
{
    String result;
    result = "{\"number\": ";
    result += number;
    result += ", \"command\": ";
    result += boolToString(valve.isCommand());
    result += ", \"faulty\": ";
    result += boolToString(valve.isFaulty());
    result += "}";
    return result;
}

String valveAdjustableJSON(ValveAdjustable valveAdjustable)
{
    String result;
    result = "\"valveAdjustable\": {\"commandOpen\": ";
    result += boolToString(valveAdjustable.isCommandOpen());
    result += ", \"commandClose\": ";
    result += boolToString(valveAdjustable.isCommandClose());
    result += ", \"faulty\": ";
    result += boolToString(valveAdjustable.isFaulty());
    result += ", \"setpoint\": ";
    result += valveAdjustable.getSetpoint();
    result += ", \"position\": ";
    result += valveAdjustable.getPosition();
    if (showSettings)
    {
        result += ", \"overtime\": ";
        result += valveAdjustable.getOvertime();
        result += ", \"limitClose\": ";
        result += valveAdjustable.getLimitClose();
        result += ", \"limitOpen\": ";
        result += valveAdjustable.getLimitOpen();
        result += ", \"deadbandClose\": ";
        result += valveAdjustable.getDeadbandClose();
        result += ", \"deadbandOpen\": ";
        result += valveAdjustable.getDeadbandOpen();
        result += ", \"deadbandPosition\": ";
        result += valveAdjustable.getDeadbandPosition();
        result += ", \"costClose\": ";
        result += valveAdjustable.getCostClose();
        result += ", \"costOpen\": ";
        result += valveAdjustable.getCostOpen();
        result += ", \"sensor\": {\"raw\": ";
        result += valveAdjustable.getPositionSensor()->getRaw();
        result += ", \"rawLowLimit\": ";
        result += valveAdjustable.getPositionSensor()->getRawLowLimit();
        result += ", \"rawHighLimit\": ";
        result += valveAdjustable.getPositionSensor()->getRawHighLimit();
        result += ", \"valueLowLimit\": ";
        result += valveAdjustable.getPositionSensor()->getValueLowLimit();
        result += ", \"valueHighLimit\": ";
        result += valveAdjustable.getPositionSensor()->getValueHighLimit();
        result += "}";
    }
    result += "}";
    return result;
}

String flowmeterJSON(Flowmeter flowmeter)
{
    String result;
    result = "\"flowmeter\": {\"flow\": ";
    result += flowmeter.getFlow();
    result += ", \"volume\": ";
    result += flowmeter.getVolume();
    if (showSettings)
    {
        result += ", \"pulsesPerLiter\": ";
        result += flowmeter.getPulsesPerLiter();
    }
    result += "}";
    return result;
}

String dispenserCollectorJSON(int number, DispenserCollector dispenserCollector)
{
    String result;

    result = "{\"number\": ";
    result += number;
    result += ", \"valves\": [";

    for (int i = 0; i < dispenserCollector.nValves_; i++)
    {
        result += valveJSON(i + 1, dispenserCollector.valves[i]);
        if (i < dispenserCollector.nValves_ - 1)
        {
            result += ",";
        }
    }
    result += "], ";
    result += valveAdjustableJSON(dispenserCollector.valveAdjustable);
    result += ", ";
    result += flowmeterJSON(dispenserCollector.flowmeter);
    result += "}";

    return result;
}

String commonJSON(ValveAdjustable valveAdjustable, Flowmeter flowmeter)
{
    String result;
    result = valveAdjustableJSON(valveAdjustable);
    result += ", ";
    result += flowmeterJSON(flowmeter);
    return result;
}

void webLoop()
{
    ws.cleanupClients();

    jsonToSend = "{\"common\": {";
    jsonToSend += commonJSON(valveAdjustable, m1);
    jsonToSend += "}, ";

    jsonToSend += "\"dispenserCollectors\": [";
    jsonToSend += dispenserCollectorJSON(1, dispenserCollector);
    jsonToSend += "]";

    jsonToSend += "}";

    notifyClients();
    Serial.println(jsonToSend);
}