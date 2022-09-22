#pragma once
#include <SimpleFOC.h>
// #include "Robot/homer/single_axis_homer.h"
// #include "Robot/homer/multi_homers.h"
#include "Robot/position_trigger/position_triggers.h"
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"
#include "CNC/mover/driver/h_bridge/h_bridge.h"
#include "eef/vsc_eef.h"

#include "MyBoards/cnc_board_base.h"
#include "pins/wroom_board.h"


class Vsc_Board: public CncBoardBase{
    public:
        // For being a real PCB board.
        Vsc_Board(){};
        void LinkEncoderSensor(Encoder* encoder){this->__motor_angle_sensor=encoder;};
        // call me must after LinkEncoderSensor().
        void Init(bool is_on_reset) override;
        void SayHello() override;

        // For being an actuator and its components.
        ActuatorDcMotor* GetActuator(EnumAxis axis_name) {return &this->__motor;};
        Encoder* GetAngleSensor(){return this->__motor_angle_sensor;};
        H_Bridge* GetMotorDriver(){return &this->__pwm_h_bridge;};

        // For being a CNC machine.
        SinglePositionTriger* GetSingleHomer(EnumAxis axis_name) override;   //{return &this->__homer_0; };
        MultiPositionHomers* GetMultiHomers(EnumAxis axis_name) override {return &this->__all_homers;};
        RobotEefBase* GetEef() override {return &this->__eef;};

        // We do nothing, just override the methods.
        void EnableMotor(EnumAxis axis_name, bool enable_it) override {};

    protected:

    private:
	    ActuatorDcMotor __motor = ActuatorDcMotor();
        PIDController __speed_pid = PIDController(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        H_Bridge __pwm_h_bridge = H_Bridge(PIN_H_BRIDGE_DIR, PIN_H_BRIDGE_SPEED);
        Encoder* __motor_angle_sensor;

        #define SMALLEST_COUNT 1
        #define BIGEST_COUNT 4096
        // MagneticSensorAnalog __sensor = MagneticSensorAnalog(PIN_SENSOR_ADC, SMALLEST_COUNT, BIGEST_COUNT);
        SinglePositionTriger __homer_0 = SinglePositionTriger(PIN_HOMER_SENSOR_HALL_0, LOW);
        SinglePositionTriger __homer_1 = SinglePositionTriger(PIN_HOMER_SENSOR_HALL_1, LOW);
        SinglePositionTriger __homer_2 = SinglePositionTriger(PIN_HOMER_SENSOR_HALL_2, LOW);
        SinglePositionTriger __homer_3 = SinglePositionTriger(PIN_HOMER_SENSOR_HALL_3, LOW);
        MultiPositionHomers __all_homers = MultiPositionHomers(4);
        Vsc_RobotEef __eef;

};