#include "board_gobot_house.h"

#define PIN_LED_A 12
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


Board_GobotHouse::Board_GobotHouse(){
    // BoardBase<LEDS_COUNT_ON_BOARD>::AppendSingleLed(0, PIN_LED_A, HIGH);
    // this->AppendSingleLed(0, PIN_LED_A, HIGH);
    // this->AppendSingleLed(0, PIN_LED_A, HIGH);
    // this->AppendSingleLed(1, PIN_LED_B, HIGH);
    // this->AppendSingleLed(2, PIN_LED_C, HIGH);
    // this->AppendSingleLed(3, PIN_LED_F, HIGH);
    // this->AppendSingleLed(4, PIN_LED_H, HIGH);

    stepper_alhpa = new Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
    stepper_beta = new Stepper(PIN_BETA_STEP, PIN_BETA_DIR);

    pinMode(PIN_ALPHA_ENABLE, OUTPUT);
    pinMode(PIN_BETA_ENABLE, OUTPUT);
    digitalWrite(PIN_ALPHA_ENABLE, LOW);
    digitalWrite(PIN_BETA_ENABLE, LOW);

}

void Board_GobotHouse::Test_home_sensor(){
  while (true){
    uint8_t state = digitalRead(PIN_HOME_ALHPA);
    digitalWrite(PIN_LED_A, state);    
    
    state = digitalRead(PIN_HOME_BETA);
    digitalWrite(PIN_LED_B, state);
  }
}
