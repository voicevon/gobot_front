#pragma once
#include "CNC/board/cnc_board_base.h"
#include <SimpleFOC.h>
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"
#include "Robot/driver/h_bridge/h_bridge.h"
// #include "CNC/mover/driver/h_bridge/h_bridge.h"
#include "Robot/Sensor/position_sensor/rotary_encoder.h"
#include "eef/vsc_eef.h"
#include "pins/wroom_board.h"


class Vsc_Board: public CncBoardBase{
    public:
        // For being a real PCB board.
        Vsc_Board(){};
        void LinkEncoderSensor(Encoder* encoder);
        // call me must after LinkEncoderSensor().
        void Init(bool is_on_reset) override;

        // For being an actuator and its components.
        // ActuatorDcMotor* GetActuator(EnumAxis_Inverseinematic axis_name) {return &this->__motor;};
        RotaryEncoder* GetAngleSensor(){return &this->__motor_angle_sensor;};
        H_Bridge* GetMotorDriver(){return &this->__pwm_h_bridge;};
        // void LinkSpeedPid_ForMotor(PIDController* speed_pid);

        // For being a CNC machine.
        RobotEefBase* GetEef() override {return &this->__eef;};
        PositionTrigger* GetPositionTrigger(uint8_t index) override;
        

        // We do nothing, just override the methods.
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) override {};

        void Test_PositionTriggers(int loops);
        void Test_HBridge(ActuatorDcMotor* motor,int loop_count);

    protected:

    private:
	    // ActuatorDcMotor __motor = ActuatorDcMotor();  //??
        H_Bridge __pwm_h_bridge = H_Bridge(PIN_H_BRIDGE_DIR, PIN_H_BRIDGE_SPEED);
        RotaryEncoder __motor_angle_sensor;
        
        PositionTrigger __all_position_triggers[4];

        Vsc_RobotEef __eef;

};