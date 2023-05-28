#pragma once
#include "MyLibs/component/binary_output_gpio.h"
// #include "CNC/reciprocator/reciprocator_base.h"
#include "Robot/reciprocator/reciprocator_base.h"

class ThreeWayValve{

    public:
        enum EnumState{
            STATE_IDLE= 1,
            STATE_GO_STRAIGHT_IDLE = 21,
            STATE_GO_STRAIGHT_SETTING = 22,
            STATE_GO_STRAIGHT_READY = 23,
            STATE_GO_STRAIGHT_INLET = 24,
            STATE_GO_STRAIGHT_OUTLET = 25,
            STATE_GO_STRAIGHT_ENDED = 26,
            STATE_GO_STARIGHT_BLOCKED = 29,
            STATE_TURN_RIGHT_IDLE = 31,
            STATE_TURN_RIGHT_SETTING = 32,
            STATE_TURN_RIGHT_READY = 33,
            STATE_TURN_RIGHT_INLET = 34,
            STATE_TURN_RIGHT_OUTLET = 35,
            STATE_TURN_RIGHT_ENDED = 36,
            STATE_TURN_RIGHT_BLOCKED = 39,
        };

        ThreeWayValve();
        void LinkReciprocator(ReciprocatorBase* reciprocator);

        void LinkIrSensors(PositionTrigger* inlet_ir_sensor, PositionTrigger* outlet_ir_sensor_go_straight, PositionTrigger* outlet_ir_sensor_turn_right);
        void LinkLeds(BinaryOutput_GPIO* led_go_straight_idle_green, BinaryOutput_GPIO* led_go_straight_inlet_red, BinaryOutput_GPIO* led_go_straight_outlet_blue,
                    BinaryOutput_GPIO* led_turn_right_idle_green, BinaryOutput_GPIO* led_turn_right_inlet_red, BinaryOutput_GPIO* led_turn_right_outlet_blue );
        void SetMode_GoStraight();
        void SetMode_TurnRight();
        void SpinOnce_Statemachine();
        EnumState GetState(){return __state;};

    protected:

    private:
        void __turn_off_all_leds();

        void __state_machine_go_straight();
        void __state_machine_turn_right();
        // TODO:  optimize to remove these
        PositionTrigger* __go_staight_stopper;
        PositionTrigger* __turn_right_stopper;

        PositionTrigger* __inlet_ir_sensor;
        PositionTrigger* __outlet_ir_sensor_go_straight;
        PositionTrigger* __outlet_ir_sensor_turn_right;

        // binaryinput

        ReciprocatorBase* __reciprocator;

        // CncActuatorBase* __actuator;
        EnumState __state = STATE_IDLE;
        BinaryOutput_GPIO* __led_go_straight_idle_green;
        BinaryOutput_GPIO* __led_go_straight_inlet_red;
        BinaryOutput_GPIO* __led_go_straight_outlet_blue;
        BinaryOutput_GPIO* __led_turn_right_idle_green;
        BinaryOutput_GPIO* __led_turn_right_inlet_red;
        BinaryOutput_GPIO* __led_turn_right_outlet_blue;
};