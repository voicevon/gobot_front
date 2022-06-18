#pragma once

#include "CNC/solution/cnc_scara/cnc_machine.h"


class GobotHouseMachine_2206: public CncScaraMachine{
    public:
        void Init(char solution_id) override;
        void PrintOut();
        int32_t GetStepsPerUnit(char axis_name) override;
        float GetHomingVelocity(char axis_name) override;

    private:
        // Fixed by electronic wiring.
        // float MOTOR_MICRO_STEPS = 16;
        // float GEAR_BOX_RATIO_ALPHA = 9.0;
        // float GEAR_BOX_RATIO_BETA = 2.6;

        float Homing_speed_beta_degree_per_second;
};


