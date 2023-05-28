#pragma once

#include "MyLibs/board/board_base.h"
#include "MyLibs/component/binary_output_gpio.h"
#include "Robot/axis_homer/position_trigger.h"
#include "CNC/reciprocator/reciprocator_base.h"


// #include "CNC/Actuator/dc_motor/h_bridge/h_bridge.h"
#include "CNC/Actuator/dc_motor/cnc_actuator_dc_motor.h"

class ThreeWayValveBoard: public BoardBase{
    public:
        ThreeWayValveBoard(){};

        void Init() override;
        // H_Bridge* GetMotor(){return &__dc_motor;};
        // ReciprocatorBase* GetReciprocator(){return &__reciprocator;};
        void SetMode_GoStraight();
        void SetMode_TurnRight();

    private:
        ReciprocatorBase __reciprocator;

        BinaryOutput_GPIO __led_go_straight_blue;
        // H_Bridge __dc_motor = H_Bridge(1,1);
        CncActuatorDcMotor __dc_motor;
        PositionTrigger __go_staight_stopper;
        PositionTrigger __turn_right_stopper;
        

};