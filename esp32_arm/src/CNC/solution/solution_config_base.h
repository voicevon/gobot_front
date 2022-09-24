#pragma once
#include <HardwareSerial.h>
#include <stdint.h>
#include "MyBoards/const.h"
#include "../coordinate/cnc_axis.h"
#include "MyLibs/basic/logger.h"
#include "axis_homer/axis_homer.h"

class CncSolutionConfigBase{
    public:
        CncSolutionConfigBase(){};
        virtual void PrintOut(const char* title);
        const bool& IsCombinedFk = _IS_COMBINED_FK;  // Like core-xy, core-yz, core-az.
        //Moving
        float GetAcceleration_ForG1(EnumAxis axis){return this->_ACCELERATION_FOR_G1[axis];};

        //Homing
        const bool& IsInverseKinematicHoimg = _home_via_inverse_kinematic;
        // float HomingSpeed(EnumAxis axis){return this->_HOMING_SPEED[axis];};
        // float HomingAcceleration(EnumAxis axis){return this->_HOMING_ACCELERATION[axis];};
        AxisHomer* GetAxisHomer(EnumAxis axis){return &this->_homers[axis];};

    protected:
        bool _IS_COMBINED_FK = false;   
        AxisHomer  _homers[CNC_AXIS_COUNT];

        //Moving
        float _ACCELERATION_FOR_G1[CNC_AXIS_COUNT];
        
        //Homing
        // int8_t _HOMING_DIR_IS_TO_MAX[CNC_AXIS_COUNT];
        // float _LONG_DISTANCE_TO_HOME[CNC_AXIS_COUNT];
        // float _HOMING_SPEED[CNC_AXIS_COUNT];
        // float _HOMING_ACCELERATION[CNC_AXIS_COUNT];
        // float _HOMED_POSITION[CNC_AXIS_COUNT];
        bool _home_via_inverse_kinematic;   //When home sensor is trigered, What is the current position? Can use predefined FK position, also can use predefined IK position.

    private:


};