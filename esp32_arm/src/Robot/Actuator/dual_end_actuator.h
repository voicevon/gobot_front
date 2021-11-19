#pragma once

#include "ActuatorBase.h"


/*
    1. Never stop at middle position.
    2. Only stop when triger is trigered.
    3. There are two endstop triger , for each end of the actuator. 
*/
class DualEndActuator: public ActuatorBase {
    public:
        void SpinOnce();
        void Init_NorthEndstop(uint8_t pin, bool triger_on_low);
        void Init_SouthEndstop(uint8_t pin, bool triger_on_low);
        void MoveToNorth();
        void MoveToSouth();
        
    protected:

    private:
        uint8_t __pin_endstop_north;
        bool __north_triger_on_low;

        uint8_t __pin_endstop_south;
        bool __south_triger_on_low;


};