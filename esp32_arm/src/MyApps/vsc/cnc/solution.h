#pragma once
#include "CNC/solution/single_axis/cnc_single_axis.h"
#include "CNC/solution/single_axis/config_base.h"
#include "../board/board_vsc.h"
#include "mover.h"

class Vsc_CncSoution: public CncSingleAxis{
    public:
        EnumAxis ConvertToEnum(char axis) override;
        void Init(Vsc_Board* board);
        // CncSolution_SingleAxisConfig* GetCncConfig() {return &this->_cnc_config;};

    protected:

    private:
        Vsc_CncMover __mover;
        // CncSolution_SingleAxisConfig _cnc_config;


};