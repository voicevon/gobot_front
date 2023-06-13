#include "board.h"


#define PIN_STATE_LED 2
#define PIN_WEB_CONFIG_BUTTON 21
#define PIN_ACTION_BUTTON 23
#define PIN_H_BRIDGE_PWM_OUTPUT 13   // Ralay1
#define PIN_RELAY_2 32   // Ralay2
#define PIN_RELAY_3 4   // Ralay3
#define PIN_RELAY_4 25   // Ralay4
#define PIN_RELAY_5 27   // Ralay5
#define PIN_RELAY_6 14   // Ralay1


#define PIN_H_BRIDGE_DIR_OUTPUT 19

void WaterDropper_Board::__InitHardware(){

    pinMode(PIN_RELAY_2, OUTPUT);
    pinMode(PIN_RELAY_3, OUTPUT);
    pinMode(PIN_RELAY_4, OUTPUT);
    pinMode(PIN_RELAY_5, OUTPUT);
    pinMode(PIN_RELAY_6, OUTPUT);

    digitalWrite(PIN_RELAY_2, LOW);
    digitalWrite(PIN_RELAY_3, LOW);
    digitalWrite(PIN_RELAY_4, LOW);
    digitalWrite(PIN_RELAY_5, LOW);
    digitalWrite(PIN_RELAY_6, LOW);
    
    __state_led.Init(0, PIN_STATE_LED, HIGH);
    __webconfig_button.Init(1,PIN_WEB_CONFIG_BUTTON, LOW);
    __action_button.Init(2, PIN_ACTION_BUTTON, LOW);
    __h_bridge.Init(PIN_H_BRIDGE_DIR_OUTPUT, PIN_H_BRIDGE_PWM_OUTPUT);
    __motor.LinkHBridgeDriver(&__h_bridge);
    // stop the motor.
    __h_bridge.SetPwmSpeed(true, 255);    

}

void WaterDropper_Board::Init(const char* app_welcome_statement){
    _InitSerial("Hi Xuming, I am WaterDropper board. Good luck......");
    Logger::Info(app_welcome_statement);
    Serial.print(COLOR_RESET);
    this->__InitHardware();
    this->_Init_SPIFFS();
    this->RepportRamUsage();
}

void WaterDropper_Board::SpinOnce(){
    __action_button.IsFired();
    __encoder->update();
}

WaterDropper_Board water_dropper_board;
