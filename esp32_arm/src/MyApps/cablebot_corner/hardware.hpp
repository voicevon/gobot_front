#ifndef __CABLEBOT_CORNER_HPP_
#define __CABLEBOT_CORNER_HPP_


#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER


#include "MyLibs/Components/Led.h"
#include "Robot/Actuator/DCDriverHBridge.h"
#include "SimpleFOC/sensors/Encoder.h"
#include "Robot/HomeTriger.h"


#define PIN_HOME_SENSOR 5
#define PIN_ENCODER_A 16
#define PIN_ENCODER_B 17
#define PIN_DC_MOTOR_A 8
#define PIN_DC_MOTOR_B 9
#define PIN_LED_POWER 22



Led led_power = Led(0,PIN_LED_POWER,LOW);
Led led_home_alpha = Led(1,2,LOW);
Encoder encoder = Encoder(PIN_ENCODER_A, PIN_ENCODER_B, 200);
DCDriverHBridge hBridge = DCDriverHBridge(PIN_DC_MOTOR_A, PIN_DC_MOTOR_B);
HomeTriger homeTriger = HomeTriger(PIN_HOME_SENSOR, HIGH);

void doA(){encoder.handleA();}
void doB(){encoder.handleB();}


void setup_hardware(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  CableBot-Corner.  Keep smiling :)");

    encoder.enableInterrupts(doA,doB);
    // board.Flash_AllLeds(3,500,500);

}

#endif
#endif
