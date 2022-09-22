#pragma once
#include <SimpleFOC.h>


class PidControllers{
    public:
        PidControllers(uint8_t pid_controller_count);
        void AppendPidController(PIDController* new_pid_controller);

    private:
        PIDController* __all_pid_controllers[5];

};