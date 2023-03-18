
//  TODO: Merge or reconstruct  this file and wifi_mqtt_client.cpp
//  This is a big improvement.



#include <HardwareSerial.h>
#include "Mqtt/remote_queue_bridge_mqtt.h"
#include "Mqtt/mqtt_message_consumer.h"
#include "mqtt_subscriber_manager.h"

RemoteQueueBridge_mqtt mono_remote_queue_bridge;




// void __connect_to_mqtt_broker(){
//     setup_wifi_mqtt();
//     while (! mqtt_is_connected){
//         delay(100);
//         Serial.print(".");
//     }
// }


/*
    Will finnally invoke(callback) ExecMattCommand(payload) when got mqtt message
    This is blocked connection, saying will not exit this function before mqtt is connected.
*/  
void mono_remote_queue_bridge_via_mqtt_setup(const char* topic, MessageQueue* local_message_queue, MqttMessageConsumer* mqtt_consumer){
    // 1. mqtt client involved
    // __connect_to_mqtt_broker();
    // g_mqttClient.onMessage(on_MqttMessage);

    // 2. message consumer and producer involved.
    mqtt_consumer->LinkLocalMq_AsMqttMessageConsumer(local_message_queue);
    mono_remote_queue_bridge.LinkLocalCommandQueue_AsMqttMessageProducer(local_message_queue);


    // 3. connect mqttclient and message receiver via subscrbe
    // Important: During this line, after subsribe_mqtt() is called, will invoke on_mqtt_message immediately.
    //            This is happened because on_mqtt_message is in other thread.
    // mono_remote_queue_bridge.Init(&g_mqttClient, topic, topic_feedback.c_str());
    MqttSubscriberManager::GetInstance().AddSubscriber(topic, &mono_remote_queue_bridge);
    String topic_feedback = String(topic) + "/fb";
    mono_remote_queue_bridge.Init(topic_feedback.c_str());

    //TODO:  re-subscibe the topic , after disconnected -->  connected.  ? is this necessary?

}

void mono_remote_queue_bridge_spin_once(){
    mono_remote_queue_bridge.SpinOnce();
}


// #endif