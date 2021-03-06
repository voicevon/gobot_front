
#include "all_applications.h"
// #include "all_devices.h"
#ifdef USING_WIFI_MQTT


//  TODO: Merge or reconstruct  this file and wifi_mqtt_client.cpp



#include <HardwareSerial.h>
#include "IoT/mqtt_syncer.h"
#include "IoT/mqtt_message_consumer.h"

uint8_t mqtt_bridge_index = 0;

struct MqttBridge{
    char mqtt_topic[20];
    MqttSyncer* mqtt_syncer;
    // MessageQueue* local_message_queue;
};
MqttBridge all_mqtt_bridges[MQTT_SYNCERS_COUNT];

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

    // dispatch topic
    if (MQTT_SYNCERS_COUNT == 1){
            // Serial.println("[Info] on_MqttMessage()  Putting remote message to local consumer." );
            all_mqtt_bridges[0].mqtt_syncer->OnReceived((const char*)payload, len);
            // Serial.println("[Info] on_MqttMessage()  Appened to mqtt_consumer.");
    }else{
        // todo: more than one syncers.
        for (int i=0; i<MQTT_SYNCERS_COUNT;i++){
            // todo:  topic is equal and copy payload, char by char
            Serial.println("[Info] on_MqttMessage()   todo: topic is equal and copy payload, char by char");
            if (all_mqtt_bridges[i].mqtt_topic == topic){
                Serial.println("[Info] on_MqttMessage()  Putting remote message to local consumer." );
            }
        }
    }
}

void setup_mqtt_block_connect(){
    setup_wifi_mqtt();
    while (! mqtt_is_connected){
        delay(100);
        Serial.print(".");
    }
}
void setup_mqtt_on_message_receive(){
    mqttClient.onMessage(on_MqttMessage);
}

/*
     Will finnally invoke(callback) ExecMattCommand(payload) when got mqtt message
*/
void append_mqtt_bridge(const char* topic, MessageQueue* local_message_queue, MqttMessageConsumer* mqtt_consumer){
    mqtt_consumer->LinkLocalMq_AsMqttMessageConsumer(local_message_queue);
    
    MqttSyncer* syncer = new MqttSyncer();
    all_mqtt_bridges[mqtt_bridge_index].mqtt_syncer = syncer;
    syncer->LinkLocalCommandQueue_AsMqttMessageProducer(local_message_queue);
    String topic_feedback = String(topic) + "/fb";
    //Important: During this line, after subsribe_mqtt() is called, will invoke on_mqtt_message immediately.
    // This is happened because on_mqtt_message is in anther thread.
    syncer->SubscribeMqtt(&mqttClient, topic, topic_feedback.c_str());

    mqtt_bridge_index++;
    if (mqtt_bridge_index > MQTT_SYNCERS_COUNT){
        Serial.println("\n\n\n\n\n");
        Serial.println("[Error] append_mqtt_link()  append too many syncers...  Not implamented more than one syncer.");
        Serial.println("\n\n\n\n\n");
    }
}

void loop_mqtt(){
    MqttSyncer* syncer;
    for (int i=0; i< MQTT_SYNCERS_COUNT; i++){
        syncer = all_mqtt_bridges[i].mqtt_syncer;
        syncer->SpinOnce();
    }
}


#endif