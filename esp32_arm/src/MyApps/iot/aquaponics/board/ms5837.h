#pragma once

#include "Mqtt/mqtt_publisher_base.h"

class Ms5837: public MqttPublisherBase{
    public:
        Ms5837(){};
        void Init();
        void SpinOnce();
        
    private:
        // return water depth in mm.
        float __ReadSensor();
        float __previous_sensor_value = 0.0f;

};