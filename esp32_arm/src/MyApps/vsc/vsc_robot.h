#pragma once

#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"
#define LAYER_COUNT 20

class VscRobot: public GcodeProducer, public MqttMessageConsumer{
    public:
        VscRobot();
        void SpinOnce(); 
        void AsyncExecuteMqttCommand(const char*) override;
        void ShowLayer(int layer_index);
        
    private:
        ActuatorDcMotor* __motor;
        float __position_diction[LAYER_COUNT];
        float __actuator_slope = 12.456;
   
};