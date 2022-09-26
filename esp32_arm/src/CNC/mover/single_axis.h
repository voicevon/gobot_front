#pragma once

#include "cnc_mover_base.h"
#include "../Actuator/actuator_base.h"
#include "CNC/planner/planner.h"




class CncMover_SingleAxis: public CncMoverBase{
    public:
        void SetEefSpeed(float speed) override;
        bool ActuatorIsMoving(EnumAxis actuator) override;

        void AllActuatorsMoveTo(uint8_t is_absolute_position, float* positions_in_cnc_unit) override;
        void SingleActuatorMoveTo(LineSegment* movement) override;
        void AllActuatorsStop() override;
        void SingleActuatorStop(EnumAxis motor_name) override;
        bool HasArrivedTargetPosition() override;

        
    protected:
        // ActuatorBase* _actuator_alpha;
    private:
};