#include "board_vsc.h"

Vsc_Board::Vsc_Board(){
    _InitSerialBoard("I am XiaoJuan.");
}

void Vsc_Board::Init(){
    __all_position_triggers[0].Init('0',PIN_HOMER_SENSOR_HALL_0, LOW);
    __all_position_triggers[1].Init('1',PIN_HOMER_SENSOR_HALL_1, LOW);
    __all_position_triggers[2].Init('2',PIN_HOMER_SENSOR_HALL_2, LOW);
    __all_position_triggers[3].Init('3',PIN_HOMER_SENSOR_HALL_3, LOW);
    PositionTrigger_Array::Instance().Init(__all_position_triggers, 4);
}

// void Vsc_Board::LinkEncoderSensor(Encoder* encoder){
//     this->__motor_angle_sensor.LinkRawSensor(encoder);
// }

void Vsc_Board::Test_PositionTriggers(int loops){
    uint32_t flags = 0;
    uint32_t last_flags = 999;
    int count =0;
    while (count < loops){
        if (flags != last_flags){
            Serial.print("Trigger is channged: \t\t");
            Serial.println(flags,BIN);
            last_flags = flags;
            count++;
        }
    }
}

void Vsc_Board::Test_HBridge(int loop_count){
    uint32_t speed = 200;
    uint32_t delay_ms = 10000;
    bool is_cw = true;
    Logger::Info("Vsc_Board::Test_HBridge");
    Logger::Print("loop_count", loop_count);
    Logger::Print("speed", speed);

    for(int i=0; i< 2 * loop_count ;i++){
        if(is_cw){
            Serial.print("\n loop count = ");
            Serial.print(i/2);
        }
        Serial.print("\t\tIs CW = ");
        Serial.print(is_cw);
        this->__pwm_h_bridge.SetPwmSpeed(is_cw, speed);
        delay(delay_ms);
        
        Serial.print("\t\tstop");
        this->__pwm_h_bridge.SetPwmSpeed(is_cw, 0);
        delay(delay_ms);
        is_cw = !is_cw;
    }
}