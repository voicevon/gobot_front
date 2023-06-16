

#include "global_const.h"
#include "von/c/utility/logger/logger.hpp"
#include "api_common.hpp"
#include "task_mqtt.h"
// #include "MyLibs/utility/logger.h"
#include "von/cpp/utility/logger.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include <AsyncMqttClient.h>



#define MQTT_HOST "voicevon.vicp.io"
#define MQTT_PORT 1883
#define MQTT_UID "von"
#define MQTT_PASSWORD "von1970"


AsyncMqttClient g_mqttClient;
static SmartMqttClient instance;


TimerHandle_t mqttReconnectTimer;
// TimerHandle_t wifiReconnectTimer;


void SmartMqttClient::connectToMqtt() {
	logInfo ("Connecting to MQTT...");
    
	g_mqttClient.setCredentials(MQTT_UID, MQTT_PASSWORD);
	g_mqttClient.connect();
}

void SmartMqttClient::Init(){
    g_mqttClient.onConnect(this->onMqttConnect);
    g_mqttClient.onDisconnect(this->onMqttDisconnect);
    g_mqttClient.onPublish(this->onMqttPublish);
    g_mqttClient.onSubscribe(this->onMqttSubscribe);
    g_mqttClient.onUnsubscribe(this->onMqttUnsubscribe);
    g_mqttClient.onMessage(this->onMqttMessage);
}
//TODO:  subsribe topics after:  disconnected --> connected.
void SmartMqttClient::onMqttConnect(bool sessionPresent) {
    logInfo("MQTT event:  onMqttConnected()");
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
    SmartMqttClient::mqtt_is_connected = true;
}

void SmartMqttClient::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    logWarn("MQTT event: onMqttDisconnect()");
    String reason_str = "   reason = ";
    if (reason == AsyncMqttClientDisconnectReason::TCP_DISCONNECTED) reason_str.concat("TCP_DISCONNECTED"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_UNACCEPTABLE_PROTOCOL_VERSION) reason_str.concat("MQTT_UNACCEPTABLE_PROTOCOL_VERSION"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_IDENTIFIER_REJECTED) reason_str.concat("MQTT_IDENTIFIER_REJECTED"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_SERVER_UNAVAILABLE) reason_str.concat("MQTT_SERVER_UNAVAILABLE"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_MALFORMED_CREDENTIALS) reason_str.concat("MQTT_MALFORMED_CREDENTIALS"); 
    if (reason == AsyncMqttClientDisconnectReason::MQTT_NOT_AUTHORIZED) reason_str.concat("MQTT_NOT_AUTHORIZED"); 
    if (reason == AsyncMqttClientDisconnectReason::ESP8266_NOT_ENOUGH_SPACE) reason_str.concat("ESP8266_NOT_ENOUGH_SPACE"); 
    if (reason == AsyncMqttClientDisconnectReason::TLS_BAD_FINGERPRINT) reason_str.concat("TLS_BAD_FINGERPRINT"); 
    logPrint_Int("reason code",(uint8_t)reason);
    logPrint_Char("reason", reason_str.c_str());

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


//Please Notice: This function will be invoked in slave thread.
// void SmartMqttClient::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
//     // https://github.com/marvinroger/async-mqtt-client/issues/39

//     bool debug = false;
//     if(debug){
//         logDebug("on_MqttMessage()   saying received.");
//         logPrint_Char("  topic: ", topic);
//         logPrint_Char("  paylod: ", payload);
//         logPrint_Int("  qos: ", properties.qos);
//         // logPrint("  dup: ", properties.dup);
//         // logPrint("  retain:", properties.retain);
//         logPrint_Int("  len: ", len);
//         logPrint_Int("  index: ", index);
//         logPrint_Int("  total: ", total);
        
//     }

//     // https://github.com/marvinroger/async-mqtt-client/issues/39
//     // if (len==total){
//     //     gs_MqttSubscriberManager::Instance().on_mqtt_client_received_message(topic, payload, len);
//     // }else{
//     //     gs_MqttSubscriberManager::Instance().on_mqtt_client_received_message(topic, payload, len, index, total);
//     // }
    
//     // logInfo("MqttClient on_MqttMessage()  Appened to mqtt_receiver. topic=");
//     // logPrint("topic", topic);
// }


void setup_wifi_mqtt_blocking_mode() {
    Serial.println("\n[Info] IoT/wifi_mqtt_client.cpp   setup_wifi_mqtt_blocking_mode()  is entering");
    Serial.println();


    // mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
    // // wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(5000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

    // // connectToWifi();
    // // WiFi.onEvent(WiFiEvent);

    // g_mqttClient.onConnect(onMqttConnect);
    // g_mqttClient.onDisconnect(onMqttDisconnect);
    // g_mqttClient.onSubscribe(onMqttSubscribe);
    // g_mqttClient.onUnsubscribe(onMqttUnsubscribe);
    // // g_mqttClient.onMessage(onMqttMessage);
    // g_mqttClient.onPublish(onMqttPublish);
    // g_mqttClient.setServer(MQTT_HOST, MQTT_PORT);

    // g_mqttClient.onMessage(on_MqttMessage);

    // // xTimerStart(wifiReconnectTimer, 0);
    // while (! g_mqttClient.connected()){
    //     delay(1000);
    //     Serial.print(". ");
    // }
    // logInfo("wifi_mqtt_clinet.cpp   setup_wifi_mqtt_blocking_mode() is finished...");

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

// SmartMqttClient instance;

void TaskMqtt(void* parameter){
    
    set_callback_mqtt_publish(instance.mqtt_publish);
    set_callback_mqtt_subscribe(instance.mqtt_subscribe);
    // set_callback_read_first_topic(mqtt_read_first_topic);
    set_callback_mqtt_read_payload(instance.mqtt_read_payload);
    set_callback_mqtt_release_buffer(instance.mqtt_release_buffer);

    while(true){
        int xx = instance.Get_Payload_bits();
        if (xx>0)        
            xEventGroupSetBits(my_EventGroup,  EVENT_BIT_MQTT_RX_0);  // set eventbit	
        // vTaskSuspend(NULL);                                            // suspend myself
        if(!g_mqttClient.connected()){
            bool timeout = false;
            // not connected, migh be connecting.
            if (timeout){

                instance.connectToMqtt();
            }
        }
        vTaskDelay(1);

    }
}
