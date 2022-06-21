#pragma once

#include "CNC/solution/cnc_scara/cnc_machine.h"


class GobotHouseMachine_2206: public CncScaraMachine{
    public:
        GobotHouseMachine_2206();
        void Init(char solution_id) override;
        void PrintOut();
        float GetHomingVelocity(char axis_name) override;

    private:

        void __InitStaticConfig();

};


