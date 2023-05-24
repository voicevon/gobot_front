#include "Mqtt/mqtt_subscriber_manager.h"

#define __JSON_BUFFER_SIZE 256  // 256 bytes for 12 leds      // https://arduinojson.org/v6/assistant/#/step1
#define __MAX_OUTPUT_LED_COUNT 12


class RemoteLedsState: public MqttSubscriberBase{
    public:
        RemoteLedsState(uint8_t leds_count);
        bool GotRemoteValue(){return __got_remote_value;};
        char* Get();

    private:
        void onGot_MqttMessage(const char* payload, uint16_t payload_len) override;
        // void __onMessage(const char* payload, uint16_t payload_len);
        char __mqtt_payload[__JSON_BUFFER_SIZE];
        bool __got_remote_value = false;
        char __remote_value[__MAX_OUTPUT_LED_COUNT];
        uint8_t __leds_count;

};