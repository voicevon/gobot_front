#pragma once
#include <SimpleFOC.h>
#include "CNC/single_axis_homer.h"
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"

#include "MyBoards/cnc_board_base.h"
#include "pins/wroom_board.h"
// #include "pins/gobot_house_board.h"


class Vsc_Board: public CncBoardBase{
    public:
        Vsc_Board(){};
        void Init(bool is_on_reset) override;
        void SayHello() override {};
        ActuatorDcMotor* GetDcMotor(){return &this->__motor;};
        MagneticSensorAnalog* GetAngleSensor(){return &this->__sensor;};
        SingleAxisHomer* GetHomer(char axis_name){return &this->__homer; };

    protected:

    private:
	    ActuatorDcMotor __motor = ActuatorDcMotor(PIN_H_BRIDGE_A, PIN_H_BRIDGE_B);

        #define SMALLEST_COUNT 1
        #define BIGEST_COUNT 4096
        MagneticSensorAnalog __sensor = MagneticSensorAnalog(PIN_SENSOR_ADC, SMALLEST_COUNT, BIGEST_COUNT);
        SingleAxisHomer __homer = SingleAxisHomer(PIN_HOMER_SENSOR_HALL, LOW);

        ActuatorStepper* GetActuator(char axis_name) override {return nullptr;};   // TODO : Feng   base shoud update.
        CncMachineBase* GetCncMechanic() override {return nullptr;};
        RobotEefBase* GetEef()override {return nullptr;};
        void EnableMotor(char axis_name, bool enable_it) override {};


};