#pragma once

class MqttPublisherBase{
    public:
        void SetMqttPublishTopic(const char * topic){_mqtt_publish_topic = topic;};

    protected:
        const char * _mqtt_publish_topic = nullptr;

};