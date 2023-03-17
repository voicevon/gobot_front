#include "mqtt_subscriber_base.h"

bool MqttSubscriberBase::IsTopicEqualTo(const char* mqtt_topic){
    // TODO: use compare function.  better be standard function or library.
    for (int i=0; i< 99; i++){
        if ((* (mqtt_topic+i)) != (* (_mqtt_topic + i))){
            return false;
        }
        if ((*(mqtt_topic + i)) == 0x00){
            // end char
            return true;
        }
    }
};

void MqttSubscriberBase::SetMqttTopic(const char* mqtt_topic){
    _mqtt_topic = mqtt_topic;
    g_mqttClient.subscribe(mqtt_topic, 2);
}
