
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT

#include <HardwareSerial.h>

//********************************************************************************************
//    MQTT and RabbitMQ
//********************************************************************************************
#include "Robot/mqtt_syncer.h"

MqttSyncer mq_sync_box_mover;
MqttSyncer mq_sync_agv;
extern AsyncMqttClient mqttClient;
bool mqtt_is_connected = false;


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


    if (topic=="boxmover/*"){
        mq_sync_box_mover.OnReceived(payload, len);
    }else{
        mq_sync_agv.OnReceived(payload, len);
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

// TODO:
// void mqtt_link(const char* topic, MessageQueue* local_mq,  MessageConsumer* local_mq_consumer){
void todo_mqtt_link(const char* topic, GcodeQueue* local_mq){
    MqttSyncer* syncer = new MqttSyncer();
    const char* topic_feedback;
    syncer->SubscribeMqtt(&mqttClient, topic, topic_feedback);
    syncer->LinkLocalCommandQueue_AsProducer(local_mq);
//     local_mq_consumer->LinkCommandQueue_AsConsumer(local_mq);
}

void mqtt_box_mover_link_gcode_queue(const char* mqtt_topic, GcodeQueue* gcode_queue){
    mq_sync_box_mover.SubscribeMqtt(&mqttClient, "garment/x2206/box", "garment/x2206/box/fb");
    mq_sync_box_mover.LinkLocalCommandQueue_AsProducer(gcode_queue);
}

void mqtt_agv_link_message_queue(const char* mqtt_topic, MessageQueue* message_queue){
    mq_sync_agv.SubscribeMqtt(&mqttClient, "garment/x2206/agv", "garment/x2206/agv/fb");
    // mq_sync_agv.LinkLocalCommandQueue(message_queue);
}


//*********************  end of MQTT and RabbitMQ  *******************************************

void loop_mqtt(){
    mq_sync_box_mover.SpinOnce();
    mq_sync_agv.SpinOnce();


    //todo
    // int SYNCERS_COUNT = 2;
    // MqttSyncer* syncer;
    // MqttSyncer* all_syncers[SYNCERS_COUNT];
    // for (int i=0; i< SYNCERS_COUNT; i++){
    //     syncer = all_syncers[i];
    //     syncer->SpinOnce();
    // }
}


#endif