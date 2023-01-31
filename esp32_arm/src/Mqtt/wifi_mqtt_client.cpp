#include "all_applications.h"
// #ifdef USING_WIFI_MQTT

#include <AsyncMqttClient.h>
#include <WiFi.h>
extern "C" {
	#include "freertos/FreeRTOS.h"
	#include "freertos/timers.h"
}

#define WIFI_SSID "Perfect"
#define WIFI_PASSWORD "1234567890"
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
TimerHandle_t wifiReconnectTimer;

void connectToWifi() {
	Serial.println("[Info] Connecting to Wi-Fi...");
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
	Serial.println("Connecting to MQTT...");

	g_mqttClient.setCredentials(MQTT_UID, MQTT_PASSWORD);
	g_mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("\n[Info] [WiFi-event] event: %d\n", event);
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        xTimerStart(wifiReconnectTimer, 0);
        break;
    default:
      break;
    }
}


//TODO:  subsribe topics after:  disconnected --> connected.
void onMqttConnect(bool sessionPresent) {
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
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
	Serial.println("Disconnected from MQTT.");

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
    wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

    WiFi.onEvent(WiFiEvent);

    g_mqttClient.onConnect(onMqttConnect);
    g_mqttClient.onDisconnect(onMqttDisconnect);
    g_mqttClient.onSubscribe(onMqttSubscribe);
    g_mqttClient.onUnsubscribe(onMqttUnsubscribe);
    // g_mqttClient.onMessage(onMqttMessage);
    g_mqttClient.onPublish(onMqttPublish);
    g_mqttClient.setServer(MQTT_HOST, MQTT_PORT);

    connectToWifi();
}

// #endif