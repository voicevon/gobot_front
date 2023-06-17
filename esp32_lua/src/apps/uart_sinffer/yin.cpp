#include <Arduino.h>
#include <stdio.h>
#include <esp_wifi.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "./main.h"
#include "./webservertask/webservertask.h"
#include "./luaenginetask/luaenginetask.h"
#include "./luademotask/luademotask.h"

#ifndef CONFIG_WEBSERVER_TASK_STACK_SIZE
#define CONFIG_WEBSERVER_TASK_STACK_SIZE 1024 * 24
#endif

#ifndef CONFIG_LUAENGINE_TASK_STACK_SIZE
#define CONFIG_LUAENGINE_TASK_STACK_SIZE 1024 * 40
#endif

#ifndef CONFIG_LUADEMO_TASK_STACK_SIZE
#define CONFIG_LUADEMO_TASK_STACK_SIZE 1024 * 16
#endif

HardwareSerial serial(0);

const char *sta_ssid = "LbnGuest"; //"LbnWX2.4";
const char *sta_password = "lbn888888";

AsyncWebServer webServer(80);
AsyncWebSocket webSocket("/ws");

TaskHandle_t webServerTaskHandle = NULL;
TaskHandle_t luaEngineTaskHandle = NULL;
TaskHandle_t luaDemoTaskHandle = NULL;

// put function declarations here:
void startWiFi();

void setup_yin()
{
  // put your setup code here, to run once:
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  serial.begin(115200, SERIAL_8N1, 5, 18);

  serial.printf("ESP32-LUA starting...\n");
  startWiFi();
  webServerInit();

  // 启动WebServer任务
  xTaskCreateUniversal(WebServerTask, "webServerTask", CONFIG_WEBSERVER_TASK_STACK_SIZE, NULL, 2, &webServerTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(luaEngineTask, "luaEngineTask", CONFIG_LUAENGINE_TASK_STACK_SIZE, NULL, 3, &luaEngineTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
  xTaskCreateUniversal(luaDemoTask, "luaDemoTask", CONFIG_LUADEMO_TASK_STACK_SIZE, NULL, 4, &luaDemoTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
}

void loop_yin()
{
  // put your main code here, to run repeatedly:
  // digitalWrite(LED_PIN, HIGH);
  vTaskDelay(1000);
  // digitalWrite(LED_PIN, LOW);
  // std::printf("...");
  // delay(1000);
}

// put function definitions here:
void startWiFi()
{
  serial.printf("connection to %s\n", sta_ssid);
  ESP_ERROR_CHECK(esp_netif_init());

  WiFi.mode(WIFI_STA);

  int count = 0;

  WiFi.begin(sta_ssid, sta_password);

  // while (WiFi.status() != WL_CONNECTED)
  while (WiFi.waitForConnectResult(8000) != WL_CONNECTED)
  {
    vTaskDelay(200);
    ++count;
    serial.printf("connecting to WiFi...%d\n", WiFi.status());
    if (count == 10)
    {
      serial.printf("local ip address is: %s\n", WiFi.localIP().toString());
      count = 0;
    }
    WiFi.begin(sta_ssid, sta_password);
  }
  serial.printf("local ip address is: %s\n", WiFi.localIP().toString());
}