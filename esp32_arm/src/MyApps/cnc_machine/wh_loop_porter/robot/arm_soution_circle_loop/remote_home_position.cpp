#include "remote_home_position.h"

void RemoteHomePosition::__onMessage(const char* payload, uint16_t payload_len){
    __remote_value = atof(payload);
    __got_remote_value = true;
}

float RemoteHomePosition::Get(){
    if (__got_remote_value){
        return __remote_value;
    }else{
        Logger::Warn("RemoteHomePosition::Get()  Not synced from remote-MQTT ");
        return 0.0f;   // TODO: return default value.
    }
}