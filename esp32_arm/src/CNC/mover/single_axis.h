#pragma once

#include "mover_base.h"
#include "../Actuator/actuator_base.h"
#include "CNC/planner/planner.h"



//TOdo:  rename to Mover_SingleActuator
class Mover_SingleAxis: public MoverBase{
    public:
        bool ActuatorIsMoving(char actuator) override;
        void AllActuatorsMoveTo(MoveBlock* move) override;
        void AllActuatorsStop() override;
        bool HasArrivedTargetPosition() override;

        
    protected:
    private:
};