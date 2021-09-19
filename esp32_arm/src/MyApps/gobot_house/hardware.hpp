#ifndef __GOBOT_HOUSE_HPP_
#define __GOBOT_HOUSE_HPP_

#define PIN_LED_POWER 12
#define PIN_LED_B 25
#define PIN_LED_C 26
#define PIN_LED_F 27
#define PIN_LED_H 14

#define PIN_HOME_ALHPA 35     //??
#define PIN_ALPHA_DIR 19
#define PIN_ALPHA_STEP 5
#define PIN_ALPHA_ENABLE 18

#define PIN_HOME_BETA 34      //??
#define PIN_BETA_DIR 17
#define PIN_BETA_STEP 4
#define PIN_BETA_ENABLE 16

#define ENDER_COIL 32
#define ENDER_COIL_EXT 33

#define PIN_MICRIO_STEP_2 21
#define PIN_MICRIO_STEP_1 22
#define PIN_MICRIO_STEP_0 23


#include "ESP32Step/src/TeensyStep.h"
#include "Robot/HomeTriger.h"
#include "MyLibs/Components/Led.h"

Led led_power = Led(0, PIN_LED_POWER, LOW);
// led_Robot = Led(2,1,LOW);
Led led_home_alpha = Led(1,2,LOW);
HomeTriger homeTriger_alpha = HomeTriger(PIN_HOME_ALHPA, HIGH);
HomeTriger homeTriger_beta = HomeTriger(PIN_HOME_BETA, HIGH);

Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
Stepper stepper_beta = Stepper(PIN_BETA_STEP, PIN_BETA_DIR);
StepControl stepControl;

// void test(){
//     auto c= &stepControl;
//     auto a= &stepper_alpha;
//     auto b= &stepper_beta;
//     c->move(*a,*b);
// }

void setup_hardware(){
    // test();
    pinMode(PIN_ALPHA_ENABLE, OUTPUT);
    pinMode(PIN_BETA_ENABLE, OUTPUT);
    pinMode(PIN_MICRIO_STEP_0, OUTPUT);
    pinMode(PIN_MICRIO_STEP_1, OUTPUT);
    pinMode(PIN_MICRIO_STEP_2, OUTPUT);

    digitalWrite(PIN_ALPHA_ENABLE, LOW);
    digitalWrite(PIN_BETA_ENABLE, LOW);
    digitalWrite(PIN_MICRIO_STEP_0, LOW);
    digitalWrite(PIN_MICRIO_STEP_1, LOW);
    digitalWrite(PIN_MICRIO_STEP_2, LOW);

}



#endif