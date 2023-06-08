#include "board.h"


#define PIN_STATE_LED 2
#define PIN_WEB_CONFIG_BUTTON 21
#define PIN_ACTION_BUTTON 23
#define PIN_H_BRIDGE_PWM_OUTPUT 18
#define PIN_H_BRIDGE_DIR_OUTPUT 19

void WaterDropper_Board::__InitHardware(){
    __state_led.Init(0, PIN_STATE_LED, HIGH);
    __webconfig_button.Init(1,PIN_WEB_CONFIG_BUTTON, LOW);
    __action_button.Init(2, PIN_ACTION_BUTTON, LOW);
    // __h_bridge.Init(PIN_H_BRIDGE_DIR_OUTPUT, PIN_H_BRIDGE_PWM_OUTPUT);
    // __motor.LinkHBridgeDriver(&__h_bridge);
}

void WaterDropper_Board::Init(const char* app_welcome_statement){
    _InitSerial("Hi Xuming, I am WaterDropper board. Good luck......");
    Logger::Info(app_welcome_statement);
    Serial.print(COLOR_RESET);
    this->__InitHardware();
    this->_Init_SPIFFS();
    this->RepportRamUsage();
}

WaterDropper_Board water_dropper_board;
