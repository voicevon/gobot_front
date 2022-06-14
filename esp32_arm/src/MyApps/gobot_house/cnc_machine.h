#pragma once

#include "CNC/solution/cnc_scara/cnc_machine.h"


class GobotHouseMachine: public CncScaraMachine{
    public:
        void Init(char solution_id) override;
        // void Init();
        void PrintOut();
        int32_t GetStepsPerUnit(char axis_name) override;
    private:

};


