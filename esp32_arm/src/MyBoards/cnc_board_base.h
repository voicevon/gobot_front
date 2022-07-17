#pragma once

#include "board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/eef/robot_eef_base.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"   //???
#include "CNC/solution/solution_config_base.h"
#include "Robot/homer/single_axis_homer.h"

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);
        virtual SingleAxisHomer* GetHomer(EnumAxis axis_name);
        virtual RobotEefBase* GetEef();
        virtual void EnableMotor(EnumAxis axis_name, bool enable_it);


    protected:

    private:

};
