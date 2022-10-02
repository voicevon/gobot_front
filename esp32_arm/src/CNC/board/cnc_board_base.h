#pragma once

#include "MyBoards/board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/eef/robot_eef_base.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"   //???
#include "Robot/axis_homer/position_trigger.h"
#include "CNC/coordinate/cnc_axis.h"
#include "MyLibs/array/pid_controllers_array.h"
#include "Robot/axis_homer/home_trigger_array.h"

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);

        virtual RobotEefBase* GetEef();
        virtual PositionTrigger* GetPositionTrigger(uint8_t index);
        virtual void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it);



    protected:

    private:

};
