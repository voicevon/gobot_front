#pragma once
#include "MyLibs/basic/board_base.h"
// #include "MyBoards/board_base.h"
// #include "CNC/board/cnc_board_base.h"
#include <SimpleFOC.h>
#include "CNC/Actuator/dc_motor/cnc_actuator_dc_motor.h"
// #include "Robot/driver/h_bridge/h_bridge.h"
#include "eef/vsc_eef.h"
#include "pins/wroom_board.h"
#include "Robot/axis_homer/position_trigger_array.h" 


class Vsc_Board: public BoardBase{
    public:
        // For being a real PCB board.
        Vsc_Board();
        void LinkEncoderSensor(Encoder* encoder){this->__encoder=encoder;};
        // call me must after LinkEncoderSensor().
        void Init() override;

        // For being an actuator and its components.
        Encoder* GetEncoder(){return this->__encoder;};
        H_Bridge* GetHBridgeDriver(){return &this->__pwm_h_bridge;};

        // For being a CNC machine.
        // RobotEefBase* GetEef() override {return &this->__eef;};

        // We do nothing, just override the methods.
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) 
        
        {};


        void Test_PositionTriggers(int loops);
        void Test_HBridge(int loop_count);

    protected:

    private:
        Encoder* __encoder;
        H_Bridge __pwm_h_bridge = H_Bridge(PIN_H_BRIDGE_DIR, PIN_H_BRIDGE_SPEED);
        Vsc_RobotEef __eef;
        
        PositionTrigger __all_position_triggers[4];

};