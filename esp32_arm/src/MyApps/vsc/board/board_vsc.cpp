#include "board_vsc.h"

void Vsc_Board::Init(bool is_on_reset){
    if (is_on_reset){
        Serial.begin(115200);
        Serial.println("I am XiaoJuan.");
    }
    //??  Init sensor ??
    this->__motor.LinkSensor(&this->__sensor);
}
