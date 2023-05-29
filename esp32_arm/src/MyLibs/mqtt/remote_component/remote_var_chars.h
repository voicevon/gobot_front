#pragma once
// #include "Mqtt/mqtt_subscriber_base.h"
#include "MyLibs/mqtt/mqtt_subscriber_base.h"

#define REMOTE_VAR_CHARS_MAX_BYTES_OF_BUFFER 60

// REMOTE_VAR_CHARS_MAX_BYTES_OF_BUFFER = 60
class RemoteVar_Chars: public MqttSubscriberBase{
    public:
        bool GotRemoteValue(){return __got_remote_value;};
        char* Get(){return __remote_value;};

    private:
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override;
        bool __got_remote_value = false;
        char __remote_value[REMOTE_VAR_CHARS_MAX_BYTES_OF_BUFFER];
        uint8_t __leds_count;

};