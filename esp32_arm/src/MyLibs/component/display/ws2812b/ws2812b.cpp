#include "ws2812b.h"

void WS2812B::Init_Remotable(const char* mqtt_topic){
    _mqtt_topic =  mqtt_topic;
}