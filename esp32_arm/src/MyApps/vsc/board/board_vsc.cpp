#include "board_vsc.h"

void Vsc_Board::Init(bool is_on_reset){
    if (is_on_reset){
        Serial.begin(115200);
        Serial.println("I am XiaoJuan.");
    }

    // this->__motor.LinkMotorDriver(&this->__pwm_h_bridge);
    // this->__motor.LinkAngleSensor(&this->__motor_angle_sensor);
    // this->__motor.PrintOut();

    __all_position_triggers[0].Init(PIN_HOMER_SENSOR_HALL_0, LOW);
    __all_position_triggers[1].Init(PIN_HOMER_SENSOR_HALL_1, LOW);
    __all_position_triggers[2].Init(PIN_HOMER_SENSOR_HALL_2, LOW);
    __all_position_triggers[3].Init(PIN_HOMER_SENSOR_HALL_3, LOW);
    HomeTrigger_Array::Instance().Init(__all_position_triggers, 4);
}

void Vsc_Board::LinkEncoderSensor(Encoder* encoder){
    this->__motor_angle_sensor.LinkRawSensor(encoder);
    // this->__motor_angle_sensor.SetupFormular(0.027173913f, 0.0f);   // slope = (10 / 56) * (56/368)
}

PositionTrigger* Vsc_Board::GetPositionTrigger(uint8_t index){
    return &__all_position_triggers[index];
    // switch (index)
    // {
    // case 1:
    //     return &this->__homer_1;
    //     break;
    // case 2:
    //     return &this->__homer_2;
    //     break;
    // case 3:
    //     return &this->__homer_3;
    //     break;
    // default:
    //     break;
    // }
    // return &this->__homer_0;
}

// void Vsc_Board::LinkSpeedPid_ForMotor(PIDController* speed_pid){
//     this->__motor.LinkPidController(speed_pid);
// }

void Vsc_Board::Test_PositionTriggers(int loops){
    uint32_t flags = 0;
    uint32_t last_flags = 999;
    int count =0;
    while (count < loops){
        // flags = this->__homer_0.IsTriggered();
        // flags += 2 * this->__homer_1.IsTriggered();
        // flags += 4 * this->__homer_2.IsTriggered();
        // flags += 8 * this->__homer_3.IsTriggered();
        if (flags != last_flags){
            Serial.print("Trigger is channged: \t\t");
            Serial.println(flags,BIN);
            last_flags = flags;
            count++;
        }
    }
}


void Vsc_Board::Test_HBridge(ActuatorDcMotor* motor,int loop_count){
    uint32_t speed = 200;
    uint32_t delay_ms = 10000;
    bool is_cw = true;
    Serial.print("speed=" + speed);
    for(int i=0; i< 2 * loop_count ;i++){
        if(is_cw)
            Serial.print("\n loop count=" + i/2);

        is_cw = !is_cw;
        Serial.print("Is CW = " + is_cw);
        this->__pwm_h_bridge.SetPwmSpeed(is_cw, speed);
        delay(delay_ms);
        
        Serial.print("\tstop");
        this->__pwm_h_bridge.SetPwmSpeed(is_cw, 0);
        delay(delay_ms);
    }
}