#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <esp_wifi.h>
#include <SPIFFS.h>
#include "webserver_task.h"

// https://randomnerdtutorials.com/esp32-websocket-server-arduino/

extern HardwareSerial mySerial;

bool ledState = 0;
const int ledPin = 2;

const char *ap_ssid = "access-point";
const char *ap_password = "12345678";

const char *sta_ssid = "LbnGuest"; //"LbnWX2.4";
const char *sta_password = "lbn888888";

IPAddress ap_local_addr(192, 168, 12, 1);
IPAddress ap_gateway(192, 168, 12, 100);
IPAddress ap_subnet(255, 255, 255, 0);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

#pragma region index.html
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
    <html lang="en-US" prefix="og: https://ogp.me/ns#" >

        <title>Richon Web Server</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="icon" href="data:,">
        <style>
            html{
                font-family: Arial, Helvetica, sans-serif;
                text-align: center;
            }
            h1{
                font-size: 1.8rem;
                color: white;
            }
            h2{
                font-size: 1.5rem;
                font-weight: bold;
                color: #14364;
            }
            .topnav{
                overflow: hidden;
                background-color: #143642;
            }
            body{
                margin: 0;
            }
            .content{
                padding: 30px;
                max-width: 600px;
                margin: 0 auto;
            }
            .card{
                background-color: #f8f7f9;
                box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
                padding-top: 10px;
                padding-bottom: 20px;
            }
            .button{
                padding: 15px 50px;
                font-size: 24px;
                text-align: center;
                outline: none;
                color: #fff;
                background-color: #0f8b8d;
                border: none;
                border-radius: 5px;
                -webkit-touch-callout: none;
                -webkit-user-select: none;
                -khtml-user-select: none;
                -moz-user-select: none;
                -ms-user-select: none;
                -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
            }
            .button:active{
                background-color: #0f8b8d;
                box-shawdow: 2 2px #cdcdcd;
                transform: translateY(2px);
            }
            .state{
                font-size: 1.5rem;
                color: #8c8c8c;
                font-weight: bold;
            }
        </style>
    </head>
    <body>
        <div class="topnav">
            <h1>Richon WebSocket Server</h1>
        </div>

        <div class="content" ondblclick = "cardDblClick(event)">
            <div class="card" id = "cardid" >
                <h2>Output-GPIO 2</h2>
                <p class="state">state: <span id="state">%STATE%</span></p>
                <p><button id="button" class="button">Toggle</button></p>
                <p>Local Ip is: %STA-IP%</p>
                <p>AP Ip is: %AP-IP%</p>
            </div>
        </div>

        <script>
            var gateway=`ws://${window.location.hostname}/ws`;
            var websocket;
            window.addEventListener("load", onLoad);

            function cardDblClick(e) {
                e.stopPropagation();
            }

            function initWebSocket(){
                console.log('Trying to open a WebSocket connection......');
                websocket = new WebSocket(gateway);
                websocket.onopen = onOpen;
                websocket.onclose = onClose;
                websocket.onmessage = onMessage;
            }

            function onOpen(event){
                console.log('Connection opened');
            }

            function onClose(event){
                console.log('Connection closed');
                setTimeout(initWebSocket, 2000);
            }

            function onMessage(event){
                var state;
                if(event.data === '1'){
                    state = "ON";
                }else{
                    state = "OFF";
                }

                document.getElementById('state').innerHTML = state;
            }

            function onLoad(event){
                initWebSocket();
                initButton();
            }

            function initButton(){
                document.getElementById('button').addEventListener('click', toggle);
            }

            function toggle(){
                websocket.send('toggle');
            }
        </script>
    </dody>
    </html>    
)rawliteral";

#pragma endregion

void notifyClients()
{
    ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        if (strcmp((char *)data, "toggle") == 0)
        {
            ledState = !ledState;
            mySerial.printf("receive data is %s\n", data);
            notifyClients();
        }
    }
}

void handleEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                 void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        mySerial.printf("WebSocket client #%u connected from %s\n", client->id(),
                        client->remoteIP().toString().c_str());
        // ws.text(client->id(), String(WiFi.localIP().toString()));
        break;
    case WS_EVT_DISCONNECT:
        mySerial.printf("WebSocket client #%u disconnected\n", client->id());
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
    ws.onEvent(handleEvent);
    server.addHandler(&ws);
}

String processor(const String &var)
{
    mySerial.println("processor is called");

    // 网页预处理
    // 网页中每个占位符（%STATE%）都调用一次处理，因此每次处理仅处理一个占位符变量的赋值即可
    if (var == "STATE")
    {
        if (ledState)
        {
            return "ON";
        }
        else
        {
            return "OFF";
        }
    };

    if (var == "STA-IP")
    {
        return WiFi.localIP().toString();
    }

    if (var == "AP-IP")
    {
        return ap_local_addr.toString();
    }

    return String();
}

void OnWiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {

    case SYSTEM_EVENT_STA_CONNECTED:
        Serial.println("ESP32 Connected to WiFi Network");
        break;
    case SYSTEM_EVENT_AP_START:
        Serial.println("ESP32 soft AP started");
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        Serial.println("Station connected to ESP32 soft AP");
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        Serial.println("Station disconnected from ESP32 soft AP");
        break;
    default:
        break;
    }
}

void WebServerTask(void *pvParam)
{
    bool hasIndex = false;
    // char indexhtml[4096];
    // memset(indexhtml, sizeof(indexhtml), 0);
    mySerial.println("WebServer Task");

    WiFi.onEvent(OnWiFiEvent);
    esp_wifi_set_mode(WIFI_MODE_AP);

    ESP_ERROR_CHECK(esp_netif_init());

    WiFi.softAP(ap_ssid, ap_password);
    WiFi.softAPConfig(ap_local_addr, ap_gateway, ap_subnet);

    WiFi.begin(sta_ssid, sta_password);
    // 只有作为STA时，在连接AP后才需要通过以下代码检测是否成功连接
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(3000);
        mySerial.printf("connecting to WiFi...%d\n", WiFi.status());
    }

    delay(1000);
    mySerial.printf("local IP Address is %s\n", WiFi.localIP().toString());
    mySerial.printf("softAP IP address is: %s\n", WiFi.softAPIP().toString());
    initWebSocket();

    // if (SPIFFS.begin(true))
    // {
    //     mySerial.println("using SPIFFS index.html");
    //     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //               { request->send(SPIFFS, "/index.html", String(), false, processor); });
    // }
    // else
    // {
    //     mySerial.println("using indexhtml ram");
    //     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //               { request->send_P(200, "text/html", index_html, processor); });
    // }

    mySerial.println("using indexhtml ram");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor); });
    server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hello from AP"); });

    server.begin(); // 启动WebServer
    for (;;)
    {
        delay(10); // 等待通过web页面Toggle指示灯
        ws.cleanupClients();
        digitalWrite(ledPin, ledState);
    }
}
