#pragma once
#include <SimpleFOC.h>
#include "Robot/homer/single_axis_homer.h"
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"
#include "eef/vsc_eef.h"

#include "MyBoards/cnc_board_base.h"
#include "pins/wroom_board.h"


class Vsc_Board: public CncBoardBase{
    public:
        Vsc_Board(){};
        void Init(bool is_on_reset) override;
        void SayHello() override;
        ActuatorDcMotor* GetActuator(EnumAxis axis_name) {return &this->__motor;};
        Encoder* GetAngleSensor(){return this->__sensor;};
        void LinkEncoderAsSensor(Encoder* encoder);
        SingleAxisHomer* GetHomer(EnumAxis axis_name) override {return &this->__homer; };
        RobotEefBase* GetEef() override {return &this->__eef;};
        void EnableMotor(EnumAxis axis_name, bool enable_it) override {};

    protected:

    private:
	    ActuatorDcMotor __motor = ActuatorDcMotor(PIN_H_BRIDGE_DIR, PIN_H_BRIDGE_SPEED);

        #define SMALLEST_COUNT 1
        #define BIGEST_COUNT 4096
        // MagneticSensorAnalog __sensor = MagneticSensorAnalog(PIN_SENSOR_ADC, SMALLEST_COUNT, BIGEST_COUNT);
        Encoder* __sensor;
        SingleAxisHomer __homer = SingleAxisHomer(PIN_HOMER_SENSOR_HALL, LOW);
        Vsc_RobotEef __eef;

};