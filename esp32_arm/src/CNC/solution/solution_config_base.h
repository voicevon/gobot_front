#pragma once
#include <HardwareSerial.h>
#include <stdint.h>
#include "MyBoards/const.h"
#include "../coordinate/cnc_axis.h"
#include "MyLibs/basic/logger.h"
#include "axis_homer/cnc_homers.h"

// TODO:  rename to KinematicConfig
class CncSolutionConfigBase{
    public:
        virtual void PrintOut(const char* title){};
        bool IsCombinedFk = false;  // Like core-xy, core-yz, core-az.
        bool IsInverseKinematicHoimg = false;

        float GetAcceleration_ForG1(EnumAxis axis){return this->_ACCELERATION_FOR_G1[axis];};
        
    protected:
        bool _IS_COMBINED_FK = false;   
        // AxisHomer  _homers[CNC_AXIS_COUNT];
        // CncHomers* _homers;
        bool _home_via_inverse_kinematic;   //When home sensor is trigered, What is the current position? Can use predefined FK position, also can use predefined IK position.

        //Moving
        float _ACCELERATION_FOR_G1[CNC_AXIS_COUNT];
        
        //Homing
        // int8_t _HOMING_DIR_IS_TO_MAX[CNC_AXIS_COUNT];
        // float _LONG_DISTANCE_TO_HOME[CNC_AXIS_COUNT];
        // float _HOMING_SPEED[CNC_AXIS_COUNT];
        // float _HOMING_ACCELERATION[CNC_AXIS_COUNT];
        // float _HOMED_POSITION[CNC_AXIS_COUNT];

    private:


};