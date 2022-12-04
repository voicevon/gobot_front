#pragma once

#include "m42_runner_switch_gpio.h"
#include "m84_runner.h"
#include "m119_runner_test_position_triggers.h"
#include "m130_runner_pid_list.h"
#include "m280_runner_servo_array.h"
#include "m408_runner_report_json.h"

class McodeOS{
    public:
        void LinkRunner(Gcode* mcode);
        bool StartToRun();  
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
        M119_Runner_TestPositionTriggers __m119_runner_test_position_triggers;
        M130_Runner_UpdatePid __m130_runner_update_pid;
        M280_Runner_Servo __m280_runner_set_servo;
        M408_Runner_ReportJson __m408_runner_report_json;
        McodeRunnerBase* __current_runner;
};