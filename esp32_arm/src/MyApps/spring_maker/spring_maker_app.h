#pragma once

#include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"


class SpringMakerApp: public GcodeProducer, public MqttMessageConsumer{
    public:
        SpringMakerApp();
        void SpinOnce(); 
        void AsyncExecuteMqttCommand(const char*) override;
        
    private:
   
};