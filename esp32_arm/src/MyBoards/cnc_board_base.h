#pragma once

#include "board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/eef/robot_eef_base.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"   //???
#include "CNC/solution/solution_config_base.h"
// #include "CNC/Sensor/position_trigger/position_triggers.h"
#include "CNC/solution/axis_homer/cnc_homers.h"
#include "MyLibs/pid_controllers/pid_controllers.h"

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);
        // virtual SinglePositionTrigger* GetSingleHomer(EnumAxis axis_name);
        // virtual AxisHomer* GetPositionTriggers(EnumAxis axis_name);

        virtual RobotEefBase* GetEef();
        virtual PidControllers* GetPidControllers(){return nullptr;};
        virtual void EnableMotor(EnumAxis axis_name, bool enable_it);
        CncHomers* GetHomers(){return &this->__all_homers;};


    protected:
        CncHomers __all_homers = CncHomers(CNC_AXIS_COUNT);

    private:

};
