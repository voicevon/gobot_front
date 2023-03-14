#include "MyLibs/basic/logger.h"
#include <AsyncMqttClient.h>
#include <WiFi.h>
#include "esp_wifi.h"

extern "C" {
	#include "freertos/FreeRTOS.h"
	#include "freertos/timers.h"
}

#define WIFI_SSID "Perfect"
#define WIFI_PASSWORD "1234567890"

// #define WIFI_SSID "CentOS"
// #define WIFI_PASSWORD "1234567890"


// #define WIFI_SSID "FuckGFW"
// #define WIFI_PASSWORD "refuckgfw"


// #define WIFI_SSID "369"
// #define WIFI_PASSWORD "hahafeng12200"

#define MQTT_HOST "voicevon.vicp.io"
#define MQTT_PORT 1883
#define MQTT_UID "von"
#define MQTT_PASSWORD "von1970"

bool mqtt_is_connected = false;
extern void app_mqtt_subscribe();
extern void app_mqtt_received_message(char* topic, char* payload);

AsyncMqttClient g_mqttClient;

TimerHandle_t mqttReconnectTimer;
// TimerHandle_t wifiReconnectTimer;


void wifi_scan_ap(){
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Logger::Info("wifi_scan_ap()  no networks found");
    } else {
        Logger::Info("wifi_scan_ap()   networks found");
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
    Logger::Info("wifi_mqtt_client.cpp  connectToWifi().");
    Logger::Print("wifi_ssid", WIFI_SSID);
    Logger::Print("wifi_password", WIFI_PASSWORD);
    Logger::Print("connectToWifi()  point", 1);
    WiFi.mode(WIFI_STA);   // cause brown-out, why?
    Logger::Print("connectToWifi()  point", 2);
    WiFi.disconnect();       //disconnect from an AP if it was previously connected     
    Logger::Print("connectToWifi()  point", 3);
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B |WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));
    Logger::Print("connectToWifi()  point", 4);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Logger::Print("connectToWifi()  point", 5);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
    delay(1000);
    }
}

void connectToMqtt() {
	Serial.println("Connecting to MQTT...");

	g_mqttClient.setCredentials(MQTT_UID, MQTT_PASSWORD);
	g_mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("\n[Info] [WiFi-event] event: %d  ", event);
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
        Serial.print("SYSTEM_EVENT_STA_GOT_IP");
        Serial.print("\tLocal IP address: ");
        Serial.println(WiFi.localIP());
        xTimerStart(mqttReconnectTimer,0);
        break;

    case SYSTEM_EVENT_STA_DISCONNECTED:
        Logger::Print("wifi_mqtt_client.cpp  WifiEvent== SYSTEM_EVENT_STA_DISCONNECTED", SYSTEM_EVENT_STA_DISCONNECTED);
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        WiFi.reconnect();
        break;
    default:
      break;
    }
}


//TODO:  subsribe topics after:  disconnected --> connected.
void onMqttConnect(bool sessionPresent) {
    Logger::Info("MQTT event:  onMqttConnected()");
    Serial.print("Session present: ");
    Serial.print(sessionPresent);
    Serial.print("\tmqtt client id:   ");
    Serial.print(g_mqttClient.getClientId());
    Serial.println();

    bool test_publish = false;
    if (test_publish){
        uint16_t packetIdSub = g_mqttClient.subscribe("test/lol", 2);
        Serial.print("Subscribing at QoS 2, packetId: ");
        Serial.println(packetIdSub);
        g_mqttClient.publish("test/lol", 0, true, "test 1");
        Serial.println("Publishing at QoS 0");
        uint16_t packetIdPub1 = g_mqttClient.publish("test/lol", 1, true, "test 2");
        Serial.print("Publishing at QoS 1, packetId: ");
        Serial.println(packetIdPub1);
        uint16_t packetIdPub2 = g_mqttClient.publish("test/lol", 2, true, "test 3");
        Serial.print("Publishing at QoS 2, packetId: ");
        Serial.println(packetIdPub2);
    }
    // If you want , add app's callback to subscribe.
    // app_mqtt_subscribe();
    mqtt_is_connected = true;
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Logger::Info("MQTT event: onMqttDisconnect()");
    String reason_str = "   reason = ";
    if (reason == AsyncMqttClientDisconnectReason::TCP_DISCONNECTED) reason_str.concat("TCP_DISCONNECTED"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_UNACCEPTABLE_PROTOCOL_VERSION) reason_str.concat("MQTT_UNACCEPTABLE_PROTOCOL_VERSION"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_IDENTIFIER_REJECTED) reason_str.concat("MQTT_IDENTIFIER_REJECTED"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_SERVER_UNAVAILABLE) reason_str.concat("MQTT_SERVER_UNAVAILABLE"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_MALFORMED_CREDENTIALS) reason_str.concat("MQTT_MALFORMED_CREDENTIALS"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_NOT_AUTHORIZED) reason_str.concat("MQTT_NOT_AUTHORIZED"); 
    if (reason == AsyncMqttClientDisconnectReason::ESP8266_NOT_ENOUGH_SPACE) reason_str.concat("ESP8266_NOT_ENOUGH_SPACE"); 
    if (reason == AsyncMqttClientDisconnectReason::TLS_BAD_FINGERPRINT) reason_str.concat("TLS_BAD_FINGERPRINT"); 
    Serial.print((uint8_t)reason);
	Serial.println(reason_str);

	if (WiFi.isConnected()) {
		xTimerStart(mqttReconnectTimer, 0);
	}
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
    bool debug = false;
    if (debug){
        Serial.println("[Info] wifi_mqtt_client.cpp   onMqttSubscribe()   Subscribe acknowledged.");
        Serial.print("  packetId: ");
        Serial.println(packetId);
        Serial.print("  qos: ");
        Serial.println(qos);
    }
}

void onMqttUnsubscribe(uint16_t packetId) {
	Serial.println("Unsubscribe acknowledged.");
	Serial.print("  packetId: ");
	Serial.println(packetId);
}

// void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
	// Serial.println("Publish received.");
	// Serial.print("  topic: ");
	// Serial.println(topic);
	// Serial.print("  paylod: ");
	// Serial.println(payload);
	// Serial.print("  qos: ");
	// Serial.println(properties.qos);
	// Serial.print("  dup: ");
	// Serial.println(properties.dup);
	// Serial.print("  retain: ");
	// Serial.println(properties.retain);
	// Serial.print("  len: ");
	// Serial.println(len);
	// Serial.print("  index: ");
	// Serial.println(index);
	// Serial.print("  total: ");
	// Serial.println(total);
	// app_mqtt_received_message(topic, payload);
	// tttt();
// }

void onMqttPublish(uint16_t packetId) {
	bool debug=false;
	if (debug){
		Serial.print("Publish acknowledged.");
		Serial.print("  packetId: ");
		Serial.println(packetId);
	}
}

void setup_wifi_mqtt() {
    Serial.println("\n[Info] IoT/wifi_mqtt_client.cpp   setup_wifi_mqtt()  is entering");
    Serial.println();


    mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
    // wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(5000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

    // connectToWifi();
    WiFi.onEvent(WiFiEvent);

    g_mqttClient.onConnect(onMqttConnect);
    g_mqttClient.onDisconnect(onMqttDisconnect);
    g_mqttClient.onSubscribe(onMqttSubscribe);
    g_mqttClient.onUnsubscribe(onMqttUnsubscribe);
    // g_mqttClient.onMessage(onMqttMessage);
    g_mqttClient.onPublish(onMqttPublish);
    g_mqttClient.setServer(MQTT_HOST, MQTT_PORT);

    connectToWifi();
    // xTimerStart(wifiReconnectTimer, 0);
}

// #endif