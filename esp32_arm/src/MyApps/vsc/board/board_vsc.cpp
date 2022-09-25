#include "board_vsc.h"

void Vsc_Board::Init(bool is_on_reset){
    if (is_on_reset){
        Serial.begin(115200);
        Serial.println("I am XiaoJuan.");
    }

    this->__motor.LinkMotorDriver(&this->__pwm_h_bridge);
    this->__motor.LinkAngleSensor(&this->__motor_angle_sensor);
    this->__motor.PrintOut();

    // motor is the user of PID controller
    this->__motor.LinkPidController(&this->__speed_pid);
    // We can send mCode 'M130 N0 P1 I2 D3'  to update pid parameters.
    this->__all_pids.AppendPidController(&this->__speed_pid);
    
    
    // this->__homer_0.SetTriggerPosition(0);
    // this->__homer_1.SetTriggerPosition(TWO_PI * 90 / 368);    //at 90 pitch of total 368 pitches.
    // this->__homer_2.SetTriggerPosition(TWO_PI * 180 / 368);    //at 180 pitch of total 368 pitches.
    // this->__homer_3.SetTriggerPosition(TWO_PI * 270 / 368);    //at 270 pitch of total 368 pitches

    // this->_cnc_homers.GetAxisHomer(AXIS_ALPHA)->AppendPositionTrigger(&this->__homer_0);
    // this->_cnc_homers.GetAxisHomer(AXIS_ALPHA)->AppendPositionTrigger(&this->__homer_1);
    // this->__all_homers.AppendPositionTrigger(&this->__homer_2);
    // this->__all_homers.AppendPositionTrigger(&this->__homer_3);

}
void Vsc_Board::LinkEncoderSensor(Encoder* encoder){
    this->__motor_angle_sensor.LinkRawSensor(encoder);
    this->__motor_angle_sensor.SetupFormular(0.027173913f, 0.0f);   // slope = (10 / 56) * (56/368)
}

PositionTrigger* Vsc_Board::GetPositionTrigger(uint8_t index){
    switch (index)
    {
    case 1:
        return &this->__homer_1;
        break;
    case 2:
        return &this->__homer_2;
        break;
    case 3:
        return &this->__homer_3;
        break;
    default:
        break;
    }
    return &this->__homer_0;
}

// PositionTrigger* Vsc_Board::GetSingleHomer(EnumAxis axis_name){
//     if (axis_name == 0){
//         return &this->__homer_0;
//     } else if (axis_name == 1){
//         return &this->__homer_1;
//     } else if (axis_name == 2){
//         return &this->__homer_2;
//     }else if (axis_name == 3){
//         return &this->__homer_3;
//     }else{
//         Logger::Error("Vsc_Board::GetSingleHomer()");
//         Logger::Print("Wrong axis_name = ", axis_name);
//     }
// }
//    //{return &this->__homer_0; };



void Vsc_Board::SayHello(){
    Logger::Debug("I am Vsc Board, Have a good day.");
}
