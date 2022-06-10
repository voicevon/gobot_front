#include "board_gobot_main.h"


void Board_GobotMain::Init(bool is_on_reset){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am Gobot-Chessboard. Good luck......");
    pinMode(PIN_ALPHA_ENABLE_2201, OUTPUT);
    pinMode(PIN_BETA_ENABLE_2201, OUTPUT);

    this->EnableMotor('A', false);
    this->EnableMotor('B', false);
    this->PIN_ROOMS[0] = PIN_SENSOR_ROOM_0;
    this->PIN_ROOMS[1] = PIN_SENSOR_ROOM_1;
    this->PIN_ROOMS[2] = PIN_SENSOR_ROOM_2;
    this->PIN_ROOMS[3] = PIN_SENSOR_ROOM_3;
    this->PIN_ROOMS[4] = PIN_SENSOR_ROOM_4;
    this->PIN_ROOMS[5] = PIN_SENSOR_ROOM_5;
    this->PIN_ROOMS[6] = PIN_SENSOR_ROOM_6;
    this->PIN_ROOMS[7] = PIN_SENSOR_ROOM_7;
    for(int i=0;i<8;i++){
        pinMode(this->PIN_ROOMS[i], INPUT_PULLUP);
    }
    this->RepportRamUsage();
}

Stepper* Board_GobotMain::GetStepper(char axis_name) {
    if (axis_name=='A'){
        return &this->stepper_alpha;
    }else if (axis_name=='B'){
        return &this->stepper_beta;
    }else{
        Serial.print("['Error']  Board_GobotMain::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }

}

SingleAxisHomer* Board_GobotMain::GetHomer(char axis_name) {
    if (axis_name=='A'){
        return &this->homer_alpha;
    }else if (axis_name=='B'){
        return &this->homer_beta;
    }else{
        Serial.print("['Error']  Board_GobotMain::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
}

RobotEef_GobotMain* Board_GobotMain::GetEef() {
    return &this->eef;
}




void Board_GobotMain::EnableMotor(char axis_name, bool enable_it) {
    Serial.print("[Info] Board_GobotMain::EnableMotor()  axis_name= ");
    Serial.print(axis_name);
    Serial.print("\t enable = ");
    Serial.println(enable_it);
    
    if (axis_name == 'A'){
        digitalWrite(PIN_ALPHA_ENABLE_2201, !enable_it);   // LOW is enable
    } else if (axis_name == 'B'){
        digitalWrite(PIN_BETA_ENABLE_2201, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] CncDriver_CoreYZ_2205::EnableMotor()  ");
    }
}

uint8_t Board_GobotMain::ReadAllRooms(){
    uint8_t result = 0;
    uint8_t p;
    for (int i=0; i<8; i++){
        p = digitalRead(PIN_ROOMS[i]);
        result += (p<<i);
    }
    return result;
}


//Return 9 is all rooms is empty.
uint8_t Board_GobotMain::GetLoadedRoom(){
    uint8_t v;
    for(uint8_t i=0; i<8; i++){
        v = digitalRead(PIN_ROOMS[i]);
        if(v == LOW)
            return i;
    }
    return 9;   
}
