#pragma once
#include <SimpleFOC.h>
#include "Robot/homer/single_axis_homer.h"
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"

#include "MyBoards/cnc_board_base.h"
#include "pins/wroom_board.h"


class Vsc_Board: public CncBoardBase{
    public:
        Vsc_Board(){};
        void Init(bool is_on_reset) override;
        void SayHello() override {};
        ActuatorBase* GetActuator(EnumAxis axis_name) override {return &this->__motor;};   //todo:  rename motor to actuator
        MagneticSensorAnalog* GetAngleSensor(){return &this->__sensor;};
        SingleAxisHomer* GetHomer(EnumAxis axis_name) override {return &this->__homer; };

    protected:

    private:
	    ActuatorDcMotor __motor = ActuatorDcMotor(PIN_H_BRIDGE_A, PIN_H_BRIDGE_B);

        #define SMALLEST_COUNT 1
        #define BIGEST_COUNT 4096
        MagneticSensorAnalog __sensor = MagneticSensorAnalog(PIN_SENSOR_ADC, SMALLEST_COUNT, BIGEST_COUNT);
        SingleAxisHomer __homer = SingleAxisHomer(PIN_HOMER_SENSOR_HALL, LOW);

        // CncSolutionConfigBase* GetCncConfig() override {return nullptr;};
        RobotEefBase* GetEef()override {return nullptr;};
        void EnableMotor(EnumAxis axis_name, bool enable_it) override {};


};