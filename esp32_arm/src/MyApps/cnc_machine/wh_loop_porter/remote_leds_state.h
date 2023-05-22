#include "Mqtt/mqtt_subscriber_manager.h"

#define __JSON_BUFFER_SIZE 60
class RemoteLedsState: public MqttSubscriberBase{
    public:
        bool GotRemoteValue(){return __got_remote_value;};
        char* Get();

    private:
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override;
        // void __onMessage(const char* payload, uint16_t payload_len);
        char __mqtt_payload[__JSON_BUFFER_SIZE];
        bool __got_remote_value = false;
        char __remote_value[7];

};