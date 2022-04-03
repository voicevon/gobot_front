
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT

#include <HardwareSerial.h>

//********************************************************************************************
//    MQTT and RabbitMQ
//********************************************************************************************
#include "Robot/mqtt_syncer.h"
#include "Robot/gcode_consumer.h"

// MqttSyncer mq_sync_box_mover;
// MqttSyncer mq_sync_agv;
extern AsyncMqttClient mqttClient;
bool mqtt_is_connected = false;

#define SYNCERS_COUNT  2
uint8_t mqtt_syncer_index = 0;
MqttSyncer* all_syncers[SYNCERS_COUNT];

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
    
    return;

    // todo dispatch topic
    // if (topic=="boxmover/*"){
    //     mq_sync_box_mover.OnReceived(payload, len);
    // }else{
    //     mq_sync_agv.OnReceived(payload, len);
    // }
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
    all_syncers[mqtt_syncer_index]= syncer;
    mqtt_syncer_index++;

    String topic_feedback =String(topic) + "/fb";
    syncer->SubscribeMqtt(&mqttClient, topic, topic_feedback.c_str());
    syncer->LinkLocalCommandQueue_AsProducer(local_gcode_queue);
    local_gcode_consumer->LinkLocalGcodeQueue_AsConsumer(local_gcode_queue);
}

// void mqtt_box_mover_link_gcode_queue(const char* mqtt_topic, GcodeQueue* gcode_queue){
//     mq_sync_box_mover.SubscribeMqtt(&mqttClient, "garment/x2206/box", "garment/x2206/box/fb");
//     mq_sync_box_mover.LinkLocalCommandQueue_AsProducer(gcode_queue);
// }

// void mqtt_agv_link_message_queue(const char* mqtt_topic, MessageQueue* message_queue){
//     mq_sync_agv.SubscribeMqtt(&mqttClient, "garment/x2206/agv", "garment/x2206/agv/fb");
//     // mq_sync_agv.LinkLocalCommandQueue(message_queue);
// }


//*********************  end of MQTT and RabbitMQ  *******************************************

void loop_mqtt(){
    // mq_sync_box_mover.SpinOnce();
    // mq_sync_agv.SpinOnce();
    MqttSyncer* syncer;
    for (int i=0; i< SYNCERS_COUNT; i++){
        syncer = all_syncers[i];
        syncer->SpinOnce();
    }
}


#endif