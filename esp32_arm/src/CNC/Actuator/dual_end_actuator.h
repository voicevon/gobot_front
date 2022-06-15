#pragma once

#include "ActuatorBase_Ver0.1.h"
#include "CNC/Actuator/DriverBase.h"


/*
    1. Never stop at middle position.
    2. Only stop when triger is trigered.
    3. There are two endstop triger , for each end of the actuator. 
*/
class DualEndActuator: public ActuatorBaseVer_0_1 {
    public:
        enum machine_state{
            START,
            MOVING_TO_NORTH,
            AT_NORTH,
            MOVING_TO_SOUTH,
            AT_SOUTH,
        };
        machine_state State;

        DualEndActuator();
        void SpinOnce();
        void Init_NorthEndstop( uint8_t pin, bool normal_is_high);
        void Init_SouthEndstop(uint8_t pin, bool normal_is_high);
        void MoveToNorth();
        void MoveToSouth();
        void Report();
        void swing_test();
    protected:
        

    private:
        uint8_t __pin_endstop_north;
        bool __north_normal_is_high;

        uint8_t __pin_endstop_south;
        bool __south_normal_is_high;


};