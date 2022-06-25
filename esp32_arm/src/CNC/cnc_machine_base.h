#pragma once
#include <HardwareSerial.h>
#include <stdint.h>
#include "MyBoards/const.h"

#define CNC_AXIS_COUNT 6


enum EnumAxis{
    ALPHA=0, 
    BETA=1, 
    GAMMA=2,
    X=3, 
    Y=4, 
    Z=5
};

#define AXIS_ANY 0
#define AXIS_ALPHA 0
#define AXIS_BETA 1
#define AXIS_GAMMA 2
#define AXIS_X 3
#define AXIS_Y 4
#define AXIS_Z 5


class CncMachineBase{
    public:
        // virtual int32_t GetStepsPerUnit(char axis_name);
        // virtual void Init(char solution_id);
        virtual void PrintOut();
        // float test(EnumAxis axis){return this->_HOMING_DIR_IS_TO_MAX[axis];};
        int8_t HomingDir_IsToMax(int8_t axis){return this->_HOMING_DIR_IS_TO_MAX[axis];};   //TODO:  axis should be an enum class.
        float HomingSpeed(int8_t axis){return this->_HOMING_SPEED[axis];};
        float HomingAcceleration(int8_t axis){return this->_HOMING_ACCELERATION[axis];};
        float HomedPosition(int8_t axis){return this->_HOMED_POSITION[axis];};
        float GetHomingVelocity(int8_t axis){return this->HomingSpeed(axis)  * this->HomingDir_IsToMax(axis);};

    protected:
        int8_t _HOMING_DIR_IS_TO_MAX[CNC_AXIS_COUNT];
        float _HOMING_SPEED[CNC_AXIS_COUNT];
        float _HOMING_ACCELERATION[CNC_AXIS_COUNT];
        float _HOMED_POSITION[CNC_AXIS_COUNT];

        // int8_t _to_int(EnumAxis axis){return 1;}
        
    private:


};