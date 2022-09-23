#pragma once
#include <SimpleFOC.h>

// Mcode example
// M130 N0 P1.1 I2.2 D3.3 J4
#define MAX_CONTROLLERS_COUNT 5

class PidControllers{
    public:
        PidControllers(uint8_t pid_controller_count);
        void AppendPidController(PIDController* new_pid_controller);
        PIDController* GetController(uint8_t index){return __all_pid_controllers[index];};
        // void Set_P(uint8_t index, float f_value);
    private:
        PIDController* __all_pid_controllers[MAX_CONTROLLERS_COUNT];
        uint8_t __next_controller_index = 0;
        uint8_t __max_controller_index;

};