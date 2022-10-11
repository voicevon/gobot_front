#pragma once

#include "MyBoards/board_base.h"
#include "ESP32Step/src/TeensyStep.h"   //todo:  remove
// #include "Robot/eef/robot_eef_base.h"          //todo:remove
#include "CNC/Actuator/stepper/actuator_stepper.h"   //todo: remove
#include "Robot/axis_homer/position_trigger.h"   // todo remove
#include "CNC/coordinate/cnc_axis.h"             //todo remvoe
#include "MyLibs/list/pid_controllers_list.h"   //todo: remove
#include "Robot/axis_homer/home_trigger_array.h"  // todo remove ??

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);

        // virtual RobotEefBase* GetEef();
        // virtual PositionTrigger* GetPositionTrigger(uint8_t index);
        virtual void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it);



    protected:

    private:

};
