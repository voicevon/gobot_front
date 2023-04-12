#include "Mqtt/mqtt_subscriber_manager.h"

class RemoteHomePosition: public MqttSubscriberBase{
    public:
        bool GotRemoteValue(){return __got_remote_value;};
        float Get();

    private:
        void __onMessage(const char* payload, uint16_t payload_len);
        float __remote_value = 0.0f;
        bool __got_remote_value = false;

};