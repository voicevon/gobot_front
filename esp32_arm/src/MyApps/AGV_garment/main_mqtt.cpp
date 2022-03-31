
#include <HardwareSerial.h>


//********************************************************************************************
//    MQTT and RabbitMQ
//********************************************************************************************
#include "Robot/mqtt_syncer.h"
MqttSyncer* mq_sync_box_mover;
MqttSyncer* mq_sync_twin_wheels;
extern AsyncMqttClient mqttClient;
bool mqtt_is_connected = false;



//This function will be invoked in slave thread.
void dispatch_MqttConnected(bool sessionPresent){
    Serial.println("\n\n     MQTT is connected !!!!\n\n");
    mqtt_is_connected = true;
    // mq_sync_box_mover->SubscribeMqtt(&mqttClient, "agv/x2206", "agv/x2206/fb");
    mq_sync_box_mover->SubscribeMqtt(&mqttClient, "agv/x2206/box", "agv/x2206/box/fb");
    mq_sync_twin_wheels->SubscribeMqtt(&mqttClient, "agv/x2206/agv", "agv/x2206/agv/fb");
}

//This function will be invoked in slave thread.
void dispatch_MqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
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
    if (1==1){
        mq_sync_box_mover->OnReceived(payload, len);
    }else{
        mq_sync_twin_wheels->OnReceived(payload, len);
    }
}
void setup_mqtt(GcodeQueue* gcode_queue, MessageQueue* message_queue){
    setup_wifi_mqtt();
    gcode_queue = new GcodeQueue();
    message_queue = new MessageQueue();
    mq_sync_box_mover = new MqttSyncer();
    mq_sync_box_mover->LinkLocalCommandQueue(gcode_queue);
    mq_sync_twin_wheels = new MqttSyncer();
    mq_sync_twin_wheels->LinkLocalCommandQueue(message_queue);
    mqttClient.onConnect(dispatch_MqttConnected);
    mqttClient.onMessage(dispatch_MqttMessage);
    while (! mqtt_is_connected){
        delay(100);
    }
}
//*********************  end of MQTT and RabbitMQ  *******************************************

void loop_mqtt(){
    mq_sync_box_mover->SpinOnce();
    mq_sync_twin_wheels->SpinOnce();
}