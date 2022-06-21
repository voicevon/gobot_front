#pragma once
#include "../board_base.h"
#include "AGV/mover_driver/single_wheel_driver/single_wheel_h_bridge_pwm_driver.h"

#define PIN_H_BRIDGE_A  11
#define PIN_H_BRIDGE_B  11

class Vsc_board: public BoardBase{
    public:
        void Init(bool is_on_reset) override{};
        void SayHello() override {};
        SingleWheel_HBridgePwmDriver* GetMotor(){return &this->__motor;};
        
    protected:

    private:
	    SingleWheel_HBridgePwmDriver __motor = SingleWheel_HBridgePwmDriver(PIN_H_BRIDGE_A, PIN_H_BRIDGE_B);


};