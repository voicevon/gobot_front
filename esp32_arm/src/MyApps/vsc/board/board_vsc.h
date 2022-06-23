#pragma once
#include "MyBoards/cnc_board_base.h"
#include "CNC/Actuator/dc_motor/actuator_dc_motor.h"
#include <SimpleFOC.h>
#include "CNC/single_axis_homer.h"

#define PIN_H_BRIDGE_A  12
#define PIN_H_BRIDGE_B  13
#define PIN_SENSOR_ADC  14
#define PIN_HOMER_SENSOR_HALL 33

class Vsc_Board: public CncBoardBase{
    public:
        Vsc_Board(){};
        void Init(bool is_on_reset) override{};
        void SayHello() override {};
        ActuatorDcMotor* GetDcMotor(){return &this->__motor;};
        MagneticSensorAnalog* GetSensor(){return &this->__sensor;};
        SingleAxisHomer* GetHomer(char axis_name){return &this->__homer; };

    protected:

    private:
	    ActuatorDcMotor __motor = ActuatorDcMotor(PIN_H_BRIDGE_A, PIN_H_BRIDGE_B);
        MagneticSensorAnalog __sensor = MagneticSensorAnalog(PIN_SENSOR_ADC);
        SingleAxisHomer __homer = SingleAxisHomer(PIN_HOMER_SENSOR_HALL, LOW);

        ActuatorStepper* GetActuator(char axis_name){};
        CncMachineBase* GetCncMechanic(){};
        RobotEefBase* GetEef(){};
        void EnableMotor(char axis_name, bool enable_it){};


};