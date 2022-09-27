#pragma once
#include <SimpleFOC.h>
#include "CNC/arm_solution/axis_homer/cnc_homers.h"
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"
#include "CNC/mover/driver/h_bridge/h_bridge.h"
#include "CNC/Sensor/position_sensor/polor_encoder.h"
#include "eef/vsc_eef.h"
#include "CNC/board/cnc_board_base.h"
#include "pins/wroom_board.h"


class Vsc_Board: public CncBoardBase{
    public:
        // For being a real PCB board.
        Vsc_Board(){};
        void LinkEncoderSensor(Encoder* encoder);
        // call me must after LinkEncoderSensor().
        void Init(bool is_on_reset) override;
        void SayHello() override;

        // For being an actuator and its components.
        ActuatorDcMotor* GetActuator(EnumAxis axis_name) {return &this->__motor;};
        PolorEncoder* GetAngleSensor(){return &this->__motor_angle_sensor;};
        H_Bridge* GetMotorDriver(){return &this->__pwm_h_bridge;};
        void LinkSpeedPid_ForMotor(PIDController* speed_pid);

        // For being a CNC machine.
        RobotEefBase* GetEef() override {return &this->__eef;};
        PositionTrigger* GetPositionTrigger(uint8_t index) override;
        

        // We do nothing, just override the methods.
        void EnableMotor(EnumAxis axis_name, bool enable_it) override {};

        void Test_PositionTriggers(int loops);

    protected:

    private:
	    ActuatorDcMotor __motor = ActuatorDcMotor();

        H_Bridge __pwm_h_bridge = H_Bridge(PIN_H_BRIDGE_DIR, PIN_H_BRIDGE_SPEED);
        PolorEncoder __motor_angle_sensor;
        
        PositionTrigger __homer_0 = PositionTrigger(PIN_HOMER_SENSOR_HALL_0, LOW);
        PositionTrigger __homer_1 = PositionTrigger(PIN_HOMER_SENSOR_HALL_1, LOW);
        PositionTrigger __homer_2 = PositionTrigger(PIN_HOMER_SENSOR_HALL_2, LOW);
        PositionTrigger __homer_3 = PositionTrigger(PIN_HOMER_SENSOR_HALL_3, LOW);

        Vsc_RobotEef __eef;

};