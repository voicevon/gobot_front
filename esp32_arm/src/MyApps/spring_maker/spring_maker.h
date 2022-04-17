#pragma once

// #include "spring_maker_hw.h"
#include "Robot/gcode_queue.h"
#include "Robot/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"


class SpringMaker: public GcodeProducer, public MqttMessageConsumer{
    public:
        SpringMaker();
        void SpinOnce(); 
        void ExecuteMqttCommand(const char*) override;
        
    private:
   
};