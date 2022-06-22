#pragma once
#include "../board_base.h"
#include "AGV/mover_driver/single_wheel_driver/single_wheel_h_bridge_pwm_driver.h"
#include "SimpleFOC.h"

#define PIN_H_BRIDGE_A  12
#define PIN_H_BRIDGE_B  13
#define PIN_SENSOR_ADC 14

class Vsc_board: public BoardBase{
    public:
        void Init(bool is_on_reset) override{};
        void SayHello() override {};
        SingleWheel_HBridgePwmDriver* GetMotor(){return &this->__motor;};
        MagneticSensorAnalog* GetSensor(){return &this->sensor;};
        
    protected:

    private:
	    SingleWheel_HBridgePwmDriver __motor = SingleWheel_HBridgePwmDriver(PIN_H_BRIDGE_A, PIN_H_BRIDGE_B);
        MagneticSensorAnalog sensor=MagneticSensorAnalog(PIN_SENSOR_ADC);

};