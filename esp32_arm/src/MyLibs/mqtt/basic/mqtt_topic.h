#pragma once
#include "MyLibs/basic/c_string/fs_filename.h"

class MqttTopic: public C_String_Base{
    public:
        // MqttTopic(): C_String_Base(__bytes, 40) {};
        MqttTopic(): C_String_Base(__bytes, 40) {};
    private:
        char __bytes[40];
};