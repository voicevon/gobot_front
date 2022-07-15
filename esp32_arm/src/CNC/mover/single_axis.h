#pragma once

#include "cnc_mover_base.h"
// #include "../Actuator/dc_motor/actuator_dc_motor.h"
#include "../Actuator/actuator_base.h"

class CncMover_SingleAxis: public CncMoverBase{
    public:
        void PrintOut(const char* title) override;
        void SetEefSpeed(float speed) override;
        bool ActuatorIsMoving(EnumAxis actuator) override;


        void AllActuatorsMoveTo(uint8_t is_absolute_position, float* positions_in_cnc_unit) override;
        void SingleActuatorMoveTo(EnumAxis actuator_name, bool is_absolute_position, float position_in_cnc_unit) override;
        void AllActuatorsStop() override;
        void SingleActuatorStop(EnumAxis motor_name) override;

        
    protected:
        // ActuatorBase* _actuator_alpha;

    private:
};