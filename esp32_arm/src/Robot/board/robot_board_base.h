#pragma once

#include "MyBoards/board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/eef/robot_eef_base.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"   //???
#include "CNC/arm_solution/axis_homer/position_trigger.h"
#include "CNC/coordinate/cnc_axis.h"
#include "MyLibs/pid_controllers/pid_controllers.h"

class RobotBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);

        virtual RobotEefBase* GetEef();
        virtual PositionTrigger* GetPositionTrigger(uint8_t index);
        virtual void EnableMotor(EnumAxis axis_name, bool enable_it);



    protected:

    private:

};