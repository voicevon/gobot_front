#pragma once

// #include "CNC/gcode/gcode_queue.h"  ???
// #include "CNC/gcode/gcode_producer.h"
// #include "IoT/mqtt_message_consumer.h"

#include "../app_base.h"


class SpringMakerApp: public AppBase{
    public:
        SpringMakerApp();
        // void SpinOnce(); 
        void AsyncExecuteMqttCommand(const char*) override;
        
    private:
   
};