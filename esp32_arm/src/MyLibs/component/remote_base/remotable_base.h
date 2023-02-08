#pragma once
#include "WString.h"

class RemotableVar_Base{
    public:

    protected:
        // char* _var_value;
        char* _mqtt_topic = nullptr;
        String _PAYLOAD_STRING_ON = "ON";   //TODO:   be static.
        String _PAYLOAD_STRING_OFF = "OFF";
// #define PAYLOAD_STRING_ON __payload_string_on
// #define PAYLOAD_STRING_OFF __payload_string_off
    private:
};




