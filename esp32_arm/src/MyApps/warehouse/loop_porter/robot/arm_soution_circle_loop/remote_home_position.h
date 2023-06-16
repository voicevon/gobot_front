// #include "Mqtt/mqtt_subscriber_manager.h"
#include "von/cpp/mqtt/subscriber/mqtt_subscriber_manager.h"

class RemoteHomePosition: public MqttSubscriberBase{
    public:
        bool GotRemoteValue(){return __got_remote_value;};
        float Get();

    private:
        void onGot_MqttMessage_whole(const char* payload, size_t payload_len) override;
        // void __onMessage(const char* payload, uint16_t payload_len);
        float __remote_value = 0.0f;
        bool __got_remote_value = false;

};