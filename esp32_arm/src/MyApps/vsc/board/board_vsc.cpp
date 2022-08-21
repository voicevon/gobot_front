#include "board_vsc.h"

void Vsc_Board::Init(bool is_on_reset){
    if (is_on_reset){
        Serial.begin(115200);
        Serial.println("I am XiaoJuan.");
    }

    this->__motor.LinkDriver(&this->__pwm_h_bridge);
    this->__motor.LinkPidController(&this->__speed_pid);

    // this->__sensor.init();
    // this->__motor.LinkSensor(&this->__sensor);
    // this->__motor.LinkSensor(this->__sensor);
    this->__motor.PrintOut();
}

void Vsc_Board::LinkEncoderSensor(Encoder* encoder){
    this->__sensor = encoder;
    this->__motor.LinkSensor(encoder);
}

void Vsc_Board::SayHello(){
    Logger::Debug("I am Vsc Board, Have a good day.");
}
