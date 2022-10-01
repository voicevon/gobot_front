#pragma once
#include <HardwareSerial.h>
#include <stdint.h>
#include "MyBoards/const.h"
#include "../coordinate/cnc_axis.h"
#include "MyLibs/basic/logger.h"
// #include "axis_homer/cnc_homers.h"
#include "Robot/axis_homer/cnc_homers.h"

class KinematicConfig{
    public:
        virtual void PrintOut(const char* title){};
        bool IsCombinedFk = false;  // Like core-xy, core-yz, core-az.
        bool IsInverseKinematicHoimg = false;

        // float GetAcceleration_ForG1(EnumAxis_Inverseinematic axis){return this->_ACCELERATION_FOR_G1[axis];};
        
    protected:
        bool _IS_COMBINED_FK = false;   
        bool _home_via_inverse_kinematic;   //When home sensor is trigered, What is the current position? Can use predefined FK position, also can use predefined IK position.

        //Moving
        float _ACCELERATION_FOR_G1[CNC_AXIS_COUNT];
        

    private:


};