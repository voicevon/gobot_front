#pragma once

#include "MyLibs/board/board_base.h"
#include "MyLibs/component/binary_output_gpio.h"
#include "CNC/Actuator/dc_motor/h_bridge/h_bridge.h"
#include "Robot/axis_homer/position_trigger.h"


class ThreeWayValveBoard: public BoardBase{
    public:
        ThreeWayValveBoard(){};

        void Init() override;
        H_Bridge* GetMotor(){return &__motor;};

    private:
        BinaryOutput_GPIO __led_go_straight_blue;
        H_Bridge __motor = H_Bridge(1,1);
        PositionTrigger __go_staight_stopper;
        PositionTrigger __turn_right_stopper;
        

};