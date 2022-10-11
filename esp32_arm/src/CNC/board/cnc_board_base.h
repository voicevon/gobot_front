#pragma once

#include "MyBoards/board_base.h"
#include "CNC/coordinate/cnc_axis.h"       

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);

        virtual void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it);



    protected:

    private:

};
