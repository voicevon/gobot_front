
#include "all_applications.h"
#ifdef USING_WIFI_MQTT


//  TODO: Merge or reconstruct  this file and wifi_mqtt_client.cpp
//  This is a big improvement.



#include <HardwareSerial.h>
#include "IoT/remote_queue_bridge_mqtt.h"
#include "IoT/mqtt_message_consumer.h"


RemoteQueueBridge_mqtt mono_remote_queue_bridge;

//Please Notice: This function will be invoked in slave thread.
void on_MqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    bool debug = false;
    if(debug){
        Serial.println("on_MqttMessage()   saying received.");
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

    Serial.println("[Info] on_MqttMessage() Dispatch message by topic,  Putting remote message to local consumer." );
    mono_remote_queue_bridge.OnReceived((const char*)payload, len);
    Serial.println("[Info] on_MqttMessage()  Appened to mqtt_consumer.");
}

void __connect_to_mqtt_broker(){
    setup_wifi_mqtt();
    while (! mqtt_is_connected){
        delay(100);
        Serial.print(".");
    }
}


/*
     Will finnally invoke(callback) ExecMattCommand(payload) when got mqtt message
*/  
void setup_mono_remote_queue_bridge_via_mqtt(const char* topic, MessageQueue* local_message_queue, MqttMessageConsumer* mqtt_consumer){
    // 1. mqtt client involved
    __connect_to_mqtt_broker();
    g_mqttClient.onMessage(on_MqttMessage);

    // 2. message consumer and producer involved.
    mqtt_consumer->LinkLocalMq_AsMqttMessageConsumer(local_message_queue);
    mono_remote_queue_bridge.LinkLocalCommandQueue_AsMqttMessageProducer(local_message_queue);


    // 3. connect mqttclient and message receiver via subscrbe
    // Important: During this line, after subsribe_mqtt() is called, will invoke on_mqtt_message immediately.
    //            This is happened because on_mqtt_message is in other thread.
    String topic_feedback = String(topic) + "/fb";
    mono_remote_queue_bridge.Init(&g_mqttClient, topic, topic_feedback.c_str());

    //TODO:  re-subscibe the topic , after disconnected -->  connected.  ? is this necessary?

}

void mono_remote_queue_bridge_spin_once(){
    mono_remote_queue_bridge.SpinOnce();
}


#endif