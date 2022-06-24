#pragma once

// #include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"
// #include "AGV/mover_driver/single_wheel_driver/single_wheel_h_bridge_pwm_driver.h"
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"


class VscRobot: public GcodeProducer, public MqttMessageConsumer{
    public:
        VscRobot();
        void SpinOnce(); 
        void ExecuteMqttCommand(const char*) override;
        
    private:
        ActuatorDcMotor* __motor;
   
};