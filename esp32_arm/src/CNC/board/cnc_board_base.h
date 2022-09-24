#pragma once

#include "MyBoards/board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/eef/robot_eef_base.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"   //???
#include "CNC/solution/axis_homer/cnc_homers.h"
#include "MyLibs/pid_controllers/pid_controllers.h"

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);

        virtual RobotEefBase* GetEef();
        virtual PidControllers* GetPidControllers(){return nullptr;};
        virtual void EnableMotor(EnumAxis axis_name, bool enable_it);
        CncHomers* GetCncHomers(){return &this->_cnc_homers;};


    protected:
        CncHomers _cnc_homers = CncHomers(CNC_AXIS_COUNT);

    private:

};
