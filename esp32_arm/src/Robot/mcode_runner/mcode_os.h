#pragma once

#include "m42_runner_switch_gpio.h"
#include "m84_runner.h"
#include "m119_runner.h"
#include "m130_runner_pid_list.h"
#include "m280_runner_servo_array.h"

class McodeOS{
    public:
        bool StartToRun(Gcode* gcode);  
        bool SpinOnce();        
        static McodeOS& Instance(){
            static McodeOS __instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return __instance;
        } 
        McodeRunnerBase* GetRunner(int mcode_id);

    private:
        M42_Runner_Switch_GPIO __m42_runner_switch_gpio;
        M84_Runner_Disable_All_Steppers __m84_runner_disable_all_steppers;
        M119_Runner_TestPositionTriggers __m119_runner;
        M130_Runner_UpdatePid __m130_runner;
        M280_Runner_Servo __m280_runner;
        McodeRunnerBase* __current_runner;
};