#include "Mqtt/mqtt_subscriber_manager.h"

#define __MIN_JSON_BUFFER_SIZE 256  // 256 bytes for 12 leds      // https://arduinojson.org/v6/assistant/#/step1
#define __MAX_OUTPUT_END_POINT_COUNT 12

class Monitoring{
    public:
        int node_index;
        int sensor_index; 
};

class RemoteString: public MqttSubscriberBase{
    public:
        RemoteString(uint8_t leds_count);
        bool GotRemoteValue(){return __got_remote_value;};
        Monitoring* Get();

    private:
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override;
        char __mqtt_payload[__MIN_JSON_BUFFER_SIZE];
        bool __got_remote_value = false;
        // char __remote_value[__MAX_OUTPUT_END_POINT_COUNT];
        Monitoring __remote_value;
        uint8_t __leds_count;

};