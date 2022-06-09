#pragma once
#include "CNC/cnc_core_yz/cnc_machine.h"

class BoxCarrierMachine: public CncCoreYZMachine{
    public:
        void Init(char solution_id) override;
        void PrintOut() override;
        int32_t GetStepsPerUnit(char axis_name) override;

    private:
        void __Init_Fast();
        void __Init_Common();

};



