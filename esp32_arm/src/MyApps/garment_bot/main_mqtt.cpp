
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT

#include <HardwareSerial.h>
#include "Robot/mqtt_syncer.h"
#include "Robot/gcode_consumer.h"

extern AsyncMqttClient mqttClient;
bool mqtt_is_connected = false;

#define SYNCERS_COUNT  2
uint8_t mqtt_syncer_index = 0;
struct mqtt_localMQ_pair{
    char mqtt_topic[20];
    MqttSyncer* mqtt_syncer;
    GcodeQueue* gcode_queue;
};
// MqttSyncer* all_syncers[SYNCERS_COUNT];
mqtt_localMQ_pair all_mqtt_syncer[2];



// Please Notice: This function will be invoked in slave thread.
void on_MqttConnected(bool sessionPresent){
    Serial.println("\n\n     MQTT is connected !!!!\n\n");
    mqtt_is_connected = true;

}

//Please Notice: This function will be invoked in slave thread.
void on_MqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    bool debug = false;
    if(debug){
        Serial.println("Publish received.");
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
    for (int i=0; i<2;i++){
        // todo:  topic is equal and copy payload, char by char
        if (all_mqtt_syncer[i].mqtt_topic == topic){
            all_mqtt_syncer[i].gcode_queue->AppendGcodeCommand(payload);
        }
    }
}

void setup_mqtt_block_connect(){
    setup_wifi_mqtt();
    mqttClient.onConnect(on_MqttConnected);
    mqttClient.onMessage(on_MqttMessage);
    while (! mqtt_is_connected){
        delay(100);
    }
}

void append_mqtt_link(const char* topic, GcodeQueue* local_gcode_queue, GcodeConsumer* local_gcode_consumer){
    MqttSyncer* syncer = new MqttSyncer();
    all_mqtt_syncer[mqtt_syncer_index].mqtt_syncer = syncer;
    all_mqtt_syncer[mqtt_syncer_index].gcode_queue = local_gcode_queue;
    // all_mqtt_syncer[mqtt_syncer_index].mqtt_topic = "1234567890123456789";  // todo copy
    mqtt_syncer_index++;

    String topic_feedback =String(topic) + "/fb";
    syncer->SubscribeMqtt(&mqttClient, topic, topic_feedback.c_str());
    syncer->LinkLocalCommandQueue_AsProducer(local_gcode_queue);
    local_gcode_consumer->LinkLocalGcodeQueue_AsConsumer(local_gcode_queue);
}


void loop_mqtt(){
    MqttSyncer* syncer;
    for (int i=0; i< SYNCERS_COUNT; i++){
        syncer = all_mqtt_syncer[i].mqtt_syncer;
        syncer->SpinOnce();
    }
}


#endif