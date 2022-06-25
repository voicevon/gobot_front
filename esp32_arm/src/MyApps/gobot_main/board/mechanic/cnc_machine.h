#pragma once

// #include "CNC/solution/cnc_scara/cnc_machine.h"
#include "CNC/solution/cnc_five_bars/cnc_machine.h"


class GoboMainMachine: public CncFiveBarMachine{
    public:
        GoboMainMachine();
        void Init(char solution_id) override;
        void PrintOut();
        float GetHomingVelocity(char axis_name) override;

    private:

        void __InitStaticConfig();

};


