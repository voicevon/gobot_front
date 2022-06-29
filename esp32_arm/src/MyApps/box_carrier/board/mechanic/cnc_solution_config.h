#pragma once
#include "CNC/solution/core_yz/cnc_machine.h"

class BoxCarrierMachine: public CncCoreYZMachine{
    public:
        void PrintOut(const char* title) override;

    private:
        void __Init_Fast();
        void __Init_Common();

};



