#pragma once
#include "remotable_base.h"

class RemoteabeSlaveBase: public RemotableVar_Base{
    public:
    protected:
        void _InitRemotable(const char* mqtt_topic){_mqtt_topic = mqtt_topic;};
    private:
        void __on_mqtt_received();
} ;