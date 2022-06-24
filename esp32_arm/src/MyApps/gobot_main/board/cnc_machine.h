#pragma once

#include "CNC/solution/cnc_scara/cnc_machine.h"


class GoboMainMachine: public CncScaraMachine{
    public:
        GoboMainMachine();
        void Init(char solution_id) override;
        void PrintOut();
        float GetHomingVelocity(char axis_name) override;

    private:

        void __InitStaticConfig();

};


