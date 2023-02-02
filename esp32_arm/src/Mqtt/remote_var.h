#pragma once

#include "WString.h"

class RemoteVar_via_Mqtt{
    public:
        RemoteVar_via_Mqtt(const char* matt_topic);

        // Call this after connected mqtt broker.
        void Start();
        char* Get(){return __value;};
        void Set(const char* new_value){strcpy(__value, new_value);};

    private:
        char*  __value;
        char* __mqtt_topic;

};