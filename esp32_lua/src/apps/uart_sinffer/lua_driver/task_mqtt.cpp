

// #include "global_const.h"
// #include "von/c/utility/logger/logger.hpp"
// #include "api_common.hpp"
#include "task_mqtt.h"
#include "von/cpp/utility/logger.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include <AsyncMqttClient.h>



#define MQTT_HOST "voicevon.vicp.io"
#define MQTT_PORT 1883
#define MQTT_UID "von"
#define MQTT_PASSWORD "von1970"


AsyncMqttClient g_mqttClient;
static TimerHandle_t mqttReconnectTimer;
static  SmartMqttClient::EnumState __ConnectionState = SmartMqttClient::EnumState::DISCONNECTED;

SmartMqttClient::EnumState SmartMqttClient::GetState(){
    return __ConnectionState;
}

void SmartMqttClient::ConnectToBroker() {
	Logger::Info ("Connecting to MQTT...");
    __ConnectionState = EnumState::CONNECTING;
    __started_at = millis();

	g_mqttClient.connect();
}

void SmartMqttClient::DisconnectFromBroker(){
    g_mqttClient.disconnect();
}

void SmartMqttClient::Init(){
    __ConnectionState = EnumState::DISCONNECTED;
    g_mqttClient.onConnect(this->onMqttConnected);
    g_mqttClient.onDisconnect(this->onMqttDisconnect);

    g_mqttClient.onPublish(this->onMqttPublish);
    g_mqttClient.onSubscribe(this->onMqttSubscribe);
    g_mqttClient.onUnsubscribe(this->onMqttUnsubscribe);
    g_mqttClient.onMessage(this->onMqttMessage);

    g_mqttClient.setServer(MQTT_HOST, MQTT_PORT);
	g_mqttClient.setCredentials(MQTT_UID, MQTT_PASSWORD);

}    
//TODO:  subsribe topics after:  disconnected --> connected.
void SmartMqttClient::onMqttConnected(bool sessionPresent) {
    __ConnectionState = EnumState::CONNECTED;
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
    // SmartMqttClient::mqtt_is_connected = true;
}

void SmartMqttClient::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    __ConnectionState = EnumState::DISCONNECTED;
    Logger::Warn("MQTT event: onMqttDisconnect()");
    String reason_str = "   reason = ";
    if (reason == AsyncMqttClientDisconnectReason::TCP_DISCONNECTED) reason_str.concat("TCP_DISCONNECTED"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_UNACCEPTABLE_PROTOCOL_VERSION) reason_str.concat("MQTT_UNACCEPTABLE_PROTOCOL_VERSION"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_IDENTIFIER_REJECTED) reason_str.concat("MQTT_IDENTIFIER_REJECTED"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_SERVER_UNAVAILABLE) reason_str.concat("MQTT_SERVER_UNAVAILABLE"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_MALFORMED_CREDENTIALS) reason_str.concat("MQTT_MALFORMED_CREDENTIALS"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_NOT_AUTHORIZED) reason_str.concat("MQTT_NOT_AUTHORIZED"); 
    if (reason == AsyncMqttClientDisconnectReason::ESP8266_NOT_ENOUGH_SPACE) reason_str.concat("ESP8266_NOT_ENOUGH_SPACE"); 
    if (reason == AsyncMqttClientDisconnectReason::TLS_BAD_FINGERPRINT) reason_str.concat("TLS_BAD_FINGERPRINT"); 
    Logger::Print("reason code",(uint8_t)reason);
    Logger::Print("reason", reason_str.c_str());

	// if (WiFi.isConnected()) {
	// 	xTimerStart(mqttReconnectTimer, 0);
	// }
}

void SmartMqttClient::onMqttSubscribe(uint16_t packetId, uint8_t qos) {
    bool debug = false;
    if (debug){
        Serial.println("[Info] wifi_mqtt_client.cpp   onMqttSubscribe()   Subscribe acknowledged.");
        Serial.print("  packetId: ");
        Serial.println(packetId);
        Serial.print("  qos: ");
        Serial.println(qos);
    }
}

void SmartMqttClient::onMqttUnsubscribe(uint16_t packetId) {
	Serial.println("Unsubscribe acknowledged.");
	Serial.print("  packetId: ");
	Serial.println(packetId);
}

void SmartMqttClient::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    bool debug = false;
    if (debug){
        Serial.println("onMqttMessage().");
        Serial.print("  topic: ");
        Serial.println(topic);
        Serial.print("  paylod: ");
        Serial.println(payload);
        Serial.print("  qos: ");
        Serial.println(properties.qos);
        Serial.print("  dup: ");
        Serial.println(properties.dup);
        Serial.print("  retain: ");
        Serial.println(properties.retain);
        Serial.print("  len: ");
        Serial.println(len);
        Serial.print("  index: ");
        Serial.println(index);
        Serial.print("  total: ");
        Serial.println(total);
    }

}

void SmartMqttClient::onMqttPublish(uint16_t packetId) {
	bool debug=false;
	if (debug){
		Serial.print("Publish acknowledged.");
		Serial.print("  packetId: ");
		Serial.println(packetId);
	}
}

void SmartMqttClient::mqtt_publish(const char* topic, const char* payload){
    g_mqttClient.publish(topic, 2,true, payload);
}

void SmartMqttClient::mqtt_subscribe(const char* topic){
    g_mqttClient.subscribe(topic, 2);
}

// const char* mqtt_read_first_topic(){

// }

int SmartMqttClient::mqtt_read_payload(const int topic_id, char* payload){
    return 1;
}

void SmartMqttClient::mqtt_release_buffer(const int topic_id){

}

void TaskMqtt(void* parameter){
    SmartMqttClient::Instance().Init();
    vTaskSuspend(NULL);
    Logger::Info("TaskMqtt is go on from init to loop");
    while(true){
        // int xx = SmartMqttClient::Instance().Get_Payload_bits();
        // if (xx>0)        
        //     xEventGroupSetBits(my_EventGroup,  EVENT_BIT_MQTT_RX_0);  // set eventbit	
        // // vTaskSuspend(NULL);                                            // suspend myself

        if(SmartMqttClient::Instance().GetState() == SmartMqttClient::EnumState::DISCONNECTED){
            SmartMqttClient::Instance().ConnectToBroker();
        }
        else if(SmartMqttClient::Instance().GetState() == SmartMqttClient::EnumState::CONNECTING){
            if (SmartMqttClient::Instance().passed_ms() > 10000){
                SmartMqttClient::Instance().DisconnectFromBroker();
                SmartMqttClient::Instance().ConnectToBroker();
            }
        }
        vTaskDelay(1);
    }
}
