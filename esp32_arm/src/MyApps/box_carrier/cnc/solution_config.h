#pragma once
#include "CNC/solution/core_yz/config_base.h"

class BoxCarrier_CncSolutionConfig: public CncSolution_CoreYZConfigBase{
    public:
        void PrintOut(const char* title) override;

    private:
        void __Init_Fast();
        void __Init_Common();

};



