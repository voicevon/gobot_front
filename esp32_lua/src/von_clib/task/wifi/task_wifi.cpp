#include <WiFi.h>
#include "esp_wifi.h"
#include "von_clib/utility/logger/logger.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"




#define WIFI_SSID "Perfect"
#define WIFI_PASSWORD "1234567890"

// #define WIFI_SSID "CentOS"
// #define WIFI_PASSWORD "1234567890"


// #define WIFI_SSID "FuckGFW"
// #define WIFI_PASSWORD "refuckgfw"


// #define WIFI_SSID "369"
// #define WIFI_PASSWORD "hahafeng12200"


void wifi_scan_ap(){
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    if (n == 0) {
        logInfo("wifi_scan_ap()  no networks found");
    } else {
        logInfo("wifi_scan_ap()   networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(":\tSSID ");
            Serial.print(WiFi.SSID(i));
            Serial.print("\t RSSI(");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
        delay(10);
    }
    }
    Serial.println("");
}

void connectToWifi() {
    logInfo("wifi_mqtt_client.cpp  connectToWifi().");
    logPrint_Char("wifi_ssid", WIFI_SSID);
    logPrint_Char("wifi_password", WIFI_PASSWORD);
    logPrint_Int("connectToWifi()  point", 1);
    WiFi.mode(WIFI_STA);   // cause brown-out, why?
    logPrint_Int("connectToWifi()  point", 2);
    WiFi.disconnect();       //disconnect from an AP if it was previously connected     
    logPrint_Int("connectToWifi()  point", 3);
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B |WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));
    logPrint_Int("connectToWifi()  point", 4);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    logPrint_Int("connectToWifi()  point", 5);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
    delay(1000);
    }
}



void WiFiEvent(WiFiEvent_t event) {
    // Serial.printf("\n[Info] [WiFi-event] event: %d  ", event);
    switch(event) {
    case SYSTEM_EVENT_WIFI_READY:
        Serial.print("\tSYSTEM_EVENT_WIFI_READY");
        break;
    case SYSTEM_EVENT_SCAN_DONE:
        Serial.print("SYSTEM_EVENT_SCAN_DONE");
        break;
    case SYSTEM_EVENT_STA_START:
        Serial.print("SYSTEM_EVENT_STA_START");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        Serial.print("SYSTEM_EVENT_STA_CONNECTED");
        break;

    case SYSTEM_EVENT_STA_GOT_IP:
        logInfo("Wifi event::SYSTEM_EVENT_STA_GOT_IP");
        // logPrint("Local IP address: ", WiFi.localIP());
        Serial.print("Local ip address is: \t");
        Serial.println(WiFi.localIP());
        // xTimerStart(mqttReconnectTimer,0);
        break;

    case SYSTEM_EVENT_STA_DISCONNECTED:
        logWarn("wifi_mqtt_client.cpp  WifiEvent== SYSTEM_EVENT_STA_DISCONNECTED");
        
        // xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        WiFi.reconnect();
        break;

    default:
      break;
    }
}

void TaskWiFi(void* parameter){
    connectToWifi();
    while (true){
        vTaskDelay(1000);
    }

}