#pragma once

#include "../sensor_base.h"

class PositionSensorBase: public SensorBase{
    public:
        virtual void SetCurrentPosition(float current_cnc_position);
        virtual float GetCurrentPosition();
};