#pragma once
/*
    This AGV is driven by 2 wheels. 
        each wheel is driven by L298N
    Track sensor is 8 bit in front of veheical frame.
        The bot connect sensor with I2C bus.
*/
#include "agv_base.h"
#include "track_sensor_i2c.h"

class AgvGarment_21a: public AgvBase{

    public:
        // void LinkWheels(ActuatorBase* left_wheel, ActuatorBase* right_wheel);
        void LinkPid(PIDController* wheel_pid);
        // void Move(AGV_DIRECTION direction, int speed_in_percent) override;
        void SpinOnce();
        void Stop() override;
   

        ActuatorBase leftWheel = ActuatorBase();
        ActuatorBase rightWheel = ActuatorBase();
        // TrackSensor_I2C* trackSensor;

    protected:
        // void _UpdateSpeed(float new_speed) override;

    private:



};


