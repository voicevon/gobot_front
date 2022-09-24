#pragma once
#include <SimpleFOC.h>
// #include "CNC/Sensor/position_trigger/position_triggers.h"
#include "CNC/solution/axis_homer/cnc_homers.h"
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"
#include "CNC/mover/driver/h_bridge/h_bridge.h"
#include "CNC/Sensor/position_sensor/polor_encoder.h"
#include "eef/vsc_eef.h"

// #include "MyBoards/cnc_board_base.h"
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

        // For being a CNC machine.
        RobotEefBase* GetEef() override {return &this->__eef;};
        PidControllers* GetPidControllers() override {return &this->__all_pids;};
        SinglePositionTrigger* GetPositionTrigger(uint8_t trigger_index);
        

        // We do nothing, just override the methods.
        void EnableMotor(EnumAxis axis_name, bool enable_it) override {};

    protected:

    private:
	    ActuatorDcMotor __motor = ActuatorDcMotor();
        PIDController __speed_pid = PIDController(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        PidControllers __all_pids = PidControllers(1);

        H_Bridge __pwm_h_bridge = H_Bridge(PIN_H_BRIDGE_DIR, PIN_H_BRIDGE_SPEED);
        // Encoder* __motor_angle_sensor;
        PolorEncoder __motor_angle_sensor;

        
        // #define SMALLEST_COUNT 1
        // #define BIGEST_COUNT 4096
        // MagneticSensorAnalog __sensor = MagneticSensorAnalog(PIN_SENSOR_ADC, SMALLEST_COUNT, BIGEST_COUNT);
        SinglePositionTrigger __homer_0 = SinglePositionTrigger(PIN_HOMER_SENSOR_HALL_0, LOW);
        SinglePositionTrigger __homer_1 = SinglePositionTrigger(PIN_HOMER_SENSOR_HALL_1, LOW);
        SinglePositionTrigger __homer_2 = SinglePositionTrigger(PIN_HOMER_SENSOR_HALL_2, LOW);
        SinglePositionTrigger __homer_3 = SinglePositionTrigger(PIN_HOMER_SENSOR_HALL_3, LOW);
        // HomingConfig __homing_config;
        Vsc_RobotEef __eef;

};