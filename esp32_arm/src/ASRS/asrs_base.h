#pragma once
#include <stdint.h>
#include <HardwareSerial.h>
#include "sensor_jetty/jetty_sensor_base.h"


class AsrsBase{
    public:
        void LinkJettySensor(JettySensorBase* jettySensor){this->jettySensor = jettySensor;};
        JettySensorBase* jettySensor;  //TODO:  be private

    protected:

    private:


};
