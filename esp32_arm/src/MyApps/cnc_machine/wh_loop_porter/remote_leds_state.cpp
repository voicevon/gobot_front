#include "remote_leds_state.h"
#include "ArduinoJson.h"
#include "MyLibs/basic/logger.h"
#include "MyLibs/basic/memory_helper.h"


RemoteLedsState::RemoteLedsState(uint8_t leds_count){
    this->__leds_count = leds_count;
}

void RemoteLedsState::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    for (int i=0; i<payload_len; i++){
        __mqtt_payload[i] = *(payload +i);
    }
    __mqtt_payload[payload_len] = 0x00;
    __got_remote_value = true;
    Logger::Debug("RemoteLedsState::onGot_MqttMessage");
    Logger::Print("__mqtt_payload", __mqtt_payload);
}

char* RemoteLedsState::Get(){
    StaticJsonDocument<__JSON_BUFFER_SIZE> doc;
    // char __mqtt_payload__[]  =  "[\"OFF\", \"ON\", \"OFF\", \"OFF\", \"OFF\", \"OFF\", \"OFF\"]";
    if (__got_remote_value){
        // Logger::Debug("RemoteLedsState::Get()");
        // Logger::Print(__mqtt_payload__, __mqtt_payload);
        // for (int i=0; i<115; i++){
        //     Serial.print(* (__mqtt_payload__ +i));
        //     Serial.print("  ");
        //     Serial.println( *(__mqtt_payload+i));
        // }
        // Logger::Print("==============================================================================","");

        DeserializationError error = deserializeJson(doc, __mqtt_payload);
        if (error) {
            Logger::Error("RemoteLedsState::Get()   deserializeJson() failed: ");
            // Logger::Print("ArduinoJson:: error code ", error);
            Serial.println(error.c_str());
            Logger::Halt("OK? Not Okay");
        }
        for(int i=0; i<__leds_count; i++){
            // if(doc[i] == 'N'){
            const char* p_char = doc[i];
            Logger::Print(p_char, i);
            if(*(p_char + 1) == 'N' ){
                __remote_value[i] = 'N';
            }else{
                __remote_value[i] = 'F';
            }
        }
        __got_remote_value = false;
    }else{
        return __remote_value;
        // Logger::Warn("RemoteLedsState::Get()  Not synced from remote-MQTT ");
    }
    return __remote_value;

}