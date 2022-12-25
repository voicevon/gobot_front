#pragma once

#include "m42_runner_switch_output.h"
#include "m84_runner.h"
#include "m119_runner_test_position_triggers.h"
#include "m130_runner_pid_list.h"
#include "m280_runner_servo_array.h"
#include "m408_runner_report_json.h"
#include "MyLibs/basic/json_loader_base.h"

class McodeOS{
    public:
        void SetupRunner(Gcode* mcode);
        // bool StartToRun();  
        bool RunnerLoop();        
        static McodeOS& Instance(){
            static McodeOS __instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return __instance;
        } 
        McodeRunnerBase* GetRunner(int mcode_id);
        void LinkRunner(M280_Runner_Servo* m280_runner){this->__m280_runner_set_servo=m280_runner;};
        void LinkRunner(M42_Runner_Switch_Output* m42_runner){this->__m42_runner_switch_output=m42_runner;};
        void LinkJsonLoader_ForM408Runner(JsonLoaderBase* json_runner){this->__m408_runner_report_json.LinkJsonLoader(json_runner);};
        // void LinkRunner(M408_Runner_ReportJson* m408_runner){&this->__m408_runner_report_json=m408_runner;};

        
    private:
        M42_Runner_Switch_Output* __m42_runner_switch_output;
        M408_Runner_ReportJson __m408_runner_report_json;
        M280_Runner_Servo*  __m280_runner_set_servo;

        M84_Runner_Disable_All_Steppers __m84_runner_disable_all_steppers;
        M119_Runner_TestPositionTriggers __m119_runner_test_position_triggers;
        M130_Runner_UpdatePid __m130_runner_update_pid;
        McodeRunnerBase* __current_runner;
};