#pragma once

#include "MyLibs/Mqtt/mqtt_publisher_base.h"
#include <Wire.h>

class Ms5837: public MqttPublisherBase{
    public:
        Ms5837(){};
        void Init(TwoWire* i2c);
        void SpinOnce();
        
    private:
        // return water depth in mm.
        float __ReadSensor();
        float __previous_sensor_value = 0.0f;
        TwoWire* __i2c;

};