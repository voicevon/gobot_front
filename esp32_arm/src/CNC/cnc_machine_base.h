#pragma once
#include <HardwareSerial.h>
#include <stdint.h>
#include "MyBoards/const.h"

class CncMachineBase{
    public:
        virtual int32_t GetStepsPerUnit(char axis_name);
        virtual void Init(char solution_id);
        virtual void PrintOut();
        
    private:


};