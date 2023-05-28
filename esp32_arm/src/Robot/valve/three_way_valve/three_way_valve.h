#pragma once
#include "MyLibs/component/binary_output_gpio.h"
#include "Robot/axis_homer/position_trigger.h"
#include "CNC/reciprocator/reciprocator_base.h"
// #include "CNC/Actuator/dc_motor/cnc_actuator_dc_motor.h"



class ThreeWayValve{

    public:

        enum EnumState{
            STATE_IDLE= 1,
            STATE_GO_STRAIGHT_IDLE = 21,
            STATE_GO_STRAIGHT_SETTING = 22,
            STATE_GO_STRAIGHT_READY = 23,
            STATE_GO_STRAIGHT_INLET = 24,
            STATE_GO_STRAIGHT_OUTLET = 25,
            STATE_GO_STARIGHT_BLOCKED = 29,
            STATE_TURN_RIGHT_IDLE = 31,
            STATE_TURN_RIGHT_SETTING = 32,
            STATE_TURN_RIGHT_READY = 33,
            STATE_TURN_RIGHT_INLET = 34,
            STATE_TURN_RIGHT_OUTLET = 35,
            STATE_TURN_RIGHT_BLOCKED = 39,
        };

        ThreeWayValve();
        void LinkReciprocator(ReciprocatorBase* reciprocator);
        // void LinkActuator(CncActuatorBase* actuator){__actuator=actuator;};
        // void LinkStopper_for_GoStraight(PositionTrigger* stopper){__go_staight_stopper=stopper;};
        // void LinkStopper_for_TurnRight(PositionTrigger* stopper){__turn_right_stopper=stopper;};
        void SetMode_GoStraight();
        void SetMode_TurnRight();
        EnumState GetState(){return __state;};

    protected:

    private:

        ReciprocatorBase* __reciprocator;
        CncActuatorBase* __actuator;
        PositionTrigger* __go_staight_stopper;
        PositionTrigger* __turn_right_stopper;
        EnumState __state = STATE_IDLE;


};