#pragma once

#include "Robot/board/board_base.h"

#include "Robot/valve/three_way_valve/three_way_valve.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger.h"
#include "Robot/reciprocator/reciprocator.h"
#include "CNC/Actuator/dc_motor/cnc_actuator_dc_motor.h"
// #include "Robot/display/digital_number/hc595_digital_number.h"
#include "Robot/board/display/digital_number/hc595_digital_number.h"

// #include "dc_motor_ir_reciprocator.h"

class BarcodeReader{
    public:
        char* GetBuffer(){return __buffer;};
        bool Read(){return false;};
    private:
        char __buffer[50];
};

class ThreeWayValveBoard: public BoardBase{
    public:
        ThreeWayValveBoard(){};
        void Init() override;
        ThreeWayValve* GetValve(){return &__valve;};
        BarcodeReader* GetBarcodeReader(){return &__barcode_reader;};
    private:
        BarcodeReader __barcode_reader;
        Hc595_Digital_number __digital_number;

        ThreeWayValve __valve;
        Reciprocator __reciprocator;
        H_Bridge __h_bridge;
        CncActuatorDcMotor __dc_motor;
        PositionTrigger __go_staight_stopper;
        PositionTrigger __turn_right_stopper;

        PositionTrigger __inlet_ir_sensor;
        PositionTrigger __outlet_ir_sensor_go_straight;
        PositionTrigger __outlet_ir_sensor_turn_right;

        BinaryOutput_GPIO __led_go_straight_blue;
        BinaryOutput_GPIO __led_go_straight_red;
        BinaryOutput_GPIO __led_go_straight_green;
        BinaryOutput_GPIO __led_turn_right_blue;
        BinaryOutput_GPIO __led_turn_right_red;
        BinaryOutput_GPIO __led_turn_right_green;

};