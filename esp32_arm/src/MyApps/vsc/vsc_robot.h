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
        void MoveTo(int layer_index, int cell_index);
        
    private:
        ActuatorDcMotor* __motor;
        int __position_in_pitch[LAYER_COUNT];
        int __GEAR_TEETH = 56;
        int __PITCH_COUNT = 157;
};