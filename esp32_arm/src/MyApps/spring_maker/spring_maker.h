#pragma once

#include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"


class SpringMaker: public GcodeProducer, public MqttMessageConsumer{
    public:
        SpringMaker();
        void SpinOnce(); 
        void AsyncExecuteMqttCommand(const char*) override;
        
    private:
   
};