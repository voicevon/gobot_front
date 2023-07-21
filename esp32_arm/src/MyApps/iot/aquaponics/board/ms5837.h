#pragma once

#include "MyLibs/Mqtt/mqtt_publisher_base.h"
#include <Wire.h>

class Ms5837: public MqttPublisherBase{
    public:
        Ms5837(){};
        void Init(TwoWire* i2c);
        void SpinOnce();

        // debug
        // float ReadSensor_data_debug(){return __ReadSensor();};
        float ReadSensor_data_debug();
        // advance
        // float* ReadSensor_Pointer = &__ReadSensor();
    private:
        // return water depth in mm.
        float __ReadSensor();
        float __previous_sensor_value = 0.0f;
        TwoWire* __i2c;
        // MS5837 __my__ms5837
};