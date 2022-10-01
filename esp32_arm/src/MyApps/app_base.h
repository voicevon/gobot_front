#pragma once


#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"


class AppBase: public GcodeProducer, public MqttMessageConsumer{
    public:
        void SpinOnce(); 

    private:

};