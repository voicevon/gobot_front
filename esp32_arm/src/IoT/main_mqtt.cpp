
#include "all_devices.h"
#ifdef USING_WIFI_MQTT


//  TODO: Merge or reconstruct  this file and wifi_mqtt_client.cpp



#include <HardwareSerial.h>
#include "IoT/mqtt_syncer.h"
#include "IoT/mqtt_message_consumer.h"

// extern AsyncMqttClient mqttClient;
// bool mqtt_is_connected = false;

uint8_t mqtt_syncer_index = 0;
struct mqtt_localMQ_pair{
    char mqtt_topic[20];
    MqttSyncer* mqtt_syncer;
    MessageQueue* local_message_queue;
};
mqtt_localMQ_pair all_mqtt_syncer[MQTT_SYNCERS_COUNT];

// Please Notice: This function will be invoked in slave thread.
// void on_MqttConnected(bool sessionPresent){
//     Serial.println("\n\n     MQTT is connected !!!!\n\n");
//     mqtt_is_connected = true;
// }

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
            Serial.println("[Info] on_MqttMessage()  Putting remote message to local consumer." );
            all_mqtt_syncer[0].local_message_queue->AppendMessage(payload);
    }else{
        // todo: more than one syncers.
        for (int i=0; i<MQTT_SYNCERS_COUNT;i++){
            // todo:  topic is equal and copy payload, char by char
            Serial.println("[Info] on_MqttMessage()   todo: topic is equal and copy payload, char by char");
            if (all_mqtt_syncer[i].mqtt_topic == topic){
                Serial.println("[Info] on_MqttMessage()  Putting remote message to local consumer." );
                all_mqtt_syncer[i].local_message_queue->AppendMessage(payload);
            }
        }
    }
}

void setup_mqtt_block_connect(){
    setup_wifi_mqtt();
    // mqttClient.onConnect(on_MqttConnected);
    mqttClient.onMessage(on_MqttMessage);
    while (! mqtt_is_connected){
        delay(100);
        Serial.print(".");
    }
}

/*
     Will invoke(callback) ExecMattCommand(payload) when got mqtt message
*/
void append_mqtt_link(const char* topic, MessageQueue* local_message_queue, MqttMessageConsumer* mqtt_consumer){
    MqttSyncer* syncer = new MqttSyncer();
    all_mqtt_syncer[mqtt_syncer_index].mqtt_syncer = syncer;
    all_mqtt_syncer[mqtt_syncer_index].local_message_queue = local_message_queue;
    // all_mqtt_syncer[mqtt_syncer_index].mqtt_topic = "1234567890123456789";  // todo copy

    String topic_feedback = String(topic) + "/fb";
    syncer->SubscribeMqtt(&mqttClient, topic, topic_feedback.c_str());
    syncer->LinkLocalCommandQueue_AsMqttMessageProducer(local_message_queue);
    mqtt_consumer->LinkLocalMq_AsMqttMessageConsumer(local_message_queue);

    mqtt_syncer_index++;
    if (mqtt_syncer_index > MQTT_SYNCERS_COUNT){
        Serial.println("\n\n\n\n\n");
        Serial.println("[Error] append_mqtt_link()  append too many syncers...  Not implamented more than one syncer.");
        Serial.println("\n\n\n\n\n");
    }
}

void loop_mqtt(){
    MqttSyncer* syncer;
    for (int i=0; i< MQTT_SYNCERS_COUNT; i++){
        syncer = all_mqtt_syncer[i].mqtt_syncer;
        syncer->SpinOnce();
    }
}


#endif