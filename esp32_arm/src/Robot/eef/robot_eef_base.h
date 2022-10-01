#pragma once

#include <HardwareSerial.h>
#include <stdint.h>
#include "MyLibs/basic/logger.h"

#define EEF_CODE_DOWN 1
#define EEF_CODE_LOWER 1
#define EEF_CODE_UP 2
#define EEF_CODE_HIGHER 2
#define EEF_CODE_SUCK 3  
#define EEF_CODE_LOAD 3
#define EEF_CODE_RELEASE 4  
#define EEF_CODE_UNLOAD 4
#define EEF_CODE_SLEEP 5


class RobotEefBase{
    public:
        virtual void Run(uint8_t eef_code);
        // virtual void PrintOut();

    private:
    
};



