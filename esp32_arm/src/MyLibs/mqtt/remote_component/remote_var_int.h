#pragma once
#include "von/utility/mqtt/subscriber/mqtt_subscriber_base.h"


class RemoteVar_Int: public MqttSubscriberBase{
    public:
        bool GotRemoteValue(){return __got_remote_value;};
        int* Get(){return &__remote_value;};

    private:
        void _onGot_MqttMessage_whole(const char* payload, size_t payload_len) override;
        bool __got_remote_value = false;
        int __remote_value;
        uint8_t __leds_count;

};