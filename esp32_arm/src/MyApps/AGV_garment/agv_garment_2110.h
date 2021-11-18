#pragma once
/*
    This AGV is driven by 2 wheels. 
        each wheel is driven by L298N
    Track sensor is 8 bit in front of veheical frame.
        The bot connect sensor with I2C bus.
*/
#include "agv_base.h"
#include "track_sensor_i2c.h"

class AgvGarment_2110: public AgvBase{

    public:
        void Init(PIDController* wheel_pid, ActuatorBase* left_wheel, ActuatorBase* right_wheel);
        // void Move(AGV_DIRECTION direction, int speed_in_percent) override;
        void SpinOnce();


        ActuatorBase* leftWheel;
        ActuatorBase* rightWheel;
        TrackSensor_I2C* lineSensor;

    protected:

    private:



};


