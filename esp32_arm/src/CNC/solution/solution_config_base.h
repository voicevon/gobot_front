#pragma once
#include <HardwareSerial.h>
#include <stdint.h>
#include "MyBoards/const.h"
// #include "coordinate/cnc_axis.h"
#include "../coordinate/cnc_axis.h"
#include "MyLibs/basic/logger.h"

class CncSolutionConfigBase{
    public:
        // virtual int32_t GetStepsPerUnit(char axis_name);
        // virtual void Init(char solution_id);
        virtual void PrintOut(const char* title);
        // float test(EnumAxis axis){return this->_HOMING_DIR_IS_TO_MAX[axis];};
        int8_t HomingDir_IsToMax(EnumAxis axis){return this->_HOMING_DIR_IS_TO_MAX[axis];};   //TODO:  axis should be an enum class.
        float HomingSpeed(EnumAxis axis){return this->_HOMING_SPEED[axis];};
        float HomingAcceleration(EnumAxis axis){return this->_HOMING_ACCELERATION[axis];};
        float HomedPosition(EnumAxis axis){return this->_HOMED_POSITION[axis];};
        float GetHomingVelocity(EnumAxis axis){return this->HomingSpeed(axis)  * this->HomingDir_IsToMax(axis);};
        const bool& IsInverseKinematicHoimg = _home_via_inverse_kinematic;
    protected:
        int8_t _HOMING_DIR_IS_TO_MAX[CNC_AXIS_COUNT];
        float _HOMING_SPEED[CNC_AXIS_COUNT];
        float _HOMING_ACCELERATION[CNC_AXIS_COUNT];
        float _HOMED_POSITION[CNC_AXIS_COUNT];
        bool _home_via_inverse_kinematic;   //When home sensor is trigered, What is the current position? Can use predefined FK position, also can use predefined IK position.

        // int8_t _to_int(EnumAxis axis){return 1;}
        
    private:


};