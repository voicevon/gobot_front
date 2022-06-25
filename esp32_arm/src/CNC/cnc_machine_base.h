#pragma once
#include <HardwareSerial.h>
#include <stdint.h>
#include "MyBoards/const.h"

#define CNC_AXIS_COUNT 6

#define AXIS_ALPHA (int8_t)(0)
#define AXIS_BETA (int8_t)(1)
#define AXIS_GAMMA 2
#define AXIS_X 3
#define AXIS_Y 4
#define AXIS_Z 5

class CncMachineBase{
    public:
        // virtual int32_t GetStepsPerUnit(char axis_name);
        virtual void Init(char solution_id);
        virtual void PrintOut();
        int8_t HomingDir_IsToMax(int8_t axis_index){return this->_HOMING_DIR_IS_TO_MAX[axis_index];};
        float HomingSpeed(int8_t axis_index){return this->_HOMING_SPEED[axis_index];};
        float HomingAcceleration(int8_t axis_index){return this->_HOMING_ACCELERATION[axis_index];};
        float HomedPosition(int8_t axis_index){return this->_HOMED_POSITION[axis_index];};
        // virtual int8_t HomingDir_IsToMax(char axis_name){return this->_HOMING_DIR_IS_TO_MAX[0];};

    protected:
        int8_t _HOMING_DIR_IS_TO_MAX[CNC_AXIS_COUNT];
        float _HOMING_SPEED[CNC_AXIS_COUNT];
        float _HOMING_ACCELERATION[CNC_AXIS_COUNT];
        float _HOMED_POSITION[CNC_AXIS_COUNT];
        
    private:


};