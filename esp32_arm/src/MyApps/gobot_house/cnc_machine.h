#pragma once

#include "CNC/cnc_scara/cnc_scara_machine.h"


class GobotHouseMachine: public CncScaraMachine{
    public:
        void Init();
        void PrintOut();
        int32_t GetStepsPerUnit(char axis_name) override;
    private:

};


