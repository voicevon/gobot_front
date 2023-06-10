#include "mqtt_subscriber_base.h"


void MqttSubscriberBase::SubscribeMqtt(const char* mqtt_topic){
    _mqtt_topic.CopyFrom(mqtt_topic);
}
