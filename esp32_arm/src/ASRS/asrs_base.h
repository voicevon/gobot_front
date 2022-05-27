#pragma once
#include <stdint.h>
#include <HardwareSerial.h>
#include "sensor_jetty/jetty_sensor_base.h"
#include "sensor_map_site/smart_rfid_reader.h"


class AsrsBase{
    public:
        JettySensorBase* jettySensor;
        SmartRfidReader* RfidReader;

    private:


};
