#include "access_point.h"

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

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

void notifyClients(String jsonToSend)
{
    ws.textAll(jsonToSend.c_str(), jsonToSend.length());
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        Serial.println((char *)data);
        jsonDeserialize(data);
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
    IPAddress IP(192, 168, 11, 1);
    IPAddress NMask(255, 255, 255, 0);
    WiFi.softAPConfig(IP, IP, NMask);

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
}

void webLoop()
{
    ws.cleanupClients();

    String jsonToSend = jsonSerialize(valveAdjustable, m1, collector, showSettings);
    notifyClients(jsonToSend);
    // Serial.println(jsonToSend);
}