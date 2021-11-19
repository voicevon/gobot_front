#pragma once
/*
    This AGV is driven by 2 wheels. 
        each wheel is driven by L298N
    Track sensor is 8 bit in front of veheical frame.
        The bot connect sensor with I2C bus.
*/
#include "AGV/agv_base.h"
#include "AGV/track_sensor_i2c.h"

class AgvGarment_21a: public AgvBase{

    public:
        void LinkPid(PIDController* wheel_pid);
        void SpinOnce();
        void Stop() override;
   

        ActuatorBase leftWheel = ActuatorBase();
        ActuatorBase rightWheel = ActuatorBase();

    protected:

    private:



};


