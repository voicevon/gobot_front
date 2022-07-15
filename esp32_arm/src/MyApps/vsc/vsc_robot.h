#pragma once

#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"


class VscRobot: public GcodeProducer, public MqttMessageConsumer{
    public:
        VscRobot();
        void SpinOnce(); 
        void AsyncExecuteMqttCommand(const char*) override;
        void ShowLayer(int layer_index);
        
    private:
        ActuatorDcMotor* __motor;
   
};