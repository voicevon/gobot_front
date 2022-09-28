#pragma once
#include "CNC/mover/dual_stepper.h"
#include "../board/board.h"

class GobotMain_CncMover: public Mover_DualStepper{
    public:
        void Init(BoardBase* board, StepControl* stepControl);
        
    private:
        ActuatorStepper __alpha;
        ActuatorStepper __beta;
};