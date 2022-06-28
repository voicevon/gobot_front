#pragma once

// #include "CNC/solution/cnc_scara/cnc_machine.h"
#include "CNC/solution/cnc_five_bars/config.h"


class GobotMainMachine: public CncFiveBarMachine{
    public:
        GobotMainMachine();
        // void Init(char solution_id) override;
        void PrintOut(const char* title) override;
        // float GetHomingVelocity(char axis_name) override;

    private:

        void __InitStaticConfig();

};


