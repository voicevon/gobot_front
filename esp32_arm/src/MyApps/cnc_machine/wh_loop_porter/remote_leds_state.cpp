#include "remote_leds_state.h"

void RemoteLedsState::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    __remote_value = atof(payload);
    __got_remote_value = true;
}

float RemoteLedsState::Get(){
    if (__got_remote_value){
        return __remote_value;
    }else{
        Logger::Warn("RemoteLedsState::Get()  Not synced from remote-MQTT ");
        return 0.0f;   // TODO: return default value.
    }
}