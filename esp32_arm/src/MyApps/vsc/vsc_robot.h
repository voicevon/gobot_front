#pragma once

// #include "CNC/gcode/gcode_queue.h"
// #include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"
#include "AGV/mover_driver/single_wheel_driver/single_wheel_h_bridge_pwm_driver.h"


class VscRobot: public MqttMessageConsumer{
    public:
        VscRobot();
        void SpinOnce(); 
        void ExecuteMqttCommand(const char*) override;
        
    private:
        SingleWheel_HBridgePwmDriver* __motor;
   
};