#include "remote_leds_state.h"
#include "ArduinoJson.h"
#include "MyLibs/basic/logger.h"
#include "MyLibs/basic/memory_helper.h"

void RemoteLedsState::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    for (int i=0; i<payload_len; i++){
        __mqtt_payload[i] = *(payload +i);
    }
    __got_remote_value = true;
}

char* RemoteLedsState::Get(){
    StaticJsonDocument<__JSON_BUFFER_SIZE> doc;
    if (__got_remote_value){
        DeserializationError error = deserializeJson(doc, __mqtt_payload);
        if (error) {
            Logger::Error("RemoteLedsState::Get()   deserializeJson() failed: ");
            Logger::Print("", error.f_str());
            Logger::Halt("OK, Not Okay");
        }
        for(int i=0; i<7; i++){
            // if(doc[i] == 'N'){
            const char* xx = doc[i];
            if( MemoryHelper::CompareSame(xx, "ON", 2)){
                __remote_value[i] = 'N';
            }else{
                __remote_value[i] = 'F';
            }
        }
    }else{
        Logger::Warn("RemoteLedsState::Get()  Not synced from remote-MQTT ");
    }
    return __remote_value;

}