#pragma once

#include "board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/eef/robot_eef_base.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"   //???
#include "CNC/solution/solution_config_base.h"
#include "Robot/position_trigger/position_triggers.h"
#include "MyLibs/pid_controllers/pid_controllers.h"

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);
        virtual SinglePositionTriger* GetSingleHomer(EnumAxis axis_name);
        virtual PositionTriggers* GetPositionTriggers(EnumAxis axis_name);

        virtual RobotEefBase* GetEef();
        virtual PidControllers* GetPidControllers();
        virtual void EnableMotor(EnumAxis axis_name, bool enable_it);


    protected:

    private:

};
