#include "board_gobot_house.h"

#define LED_A 12
#define LED_B 25
#define LED_C 26
#define LED_F 27
#define LED_H 14

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
    AppendSingleLed(0, LED_A,HIGH);
    AppendSingleLed(1, LED_B,HIGH);
    AppendSingleLed(2, LED_C,HIGH);
    AppendSingleLed(3, LED_F,HIGH);
    AppendSingleLed(4, LED_H,HIGH);

    alhpa = new StepperDriver(PIN_ALPHA_STEP, PIN_ALPHA_DIR,PIN_ALPHA_ENABLE);
    beta = new StepperDriver(PIN_BETA_STEP, PIN_BETA_DIR, PIN_BETA_ENABLE);
}

void Board_GobotHouse::Test_home_sensor(){
  while (true){
    uint8_t state = digitalRead(PIN_HOME_ALHPA);
    digitalWrite(LED_A, state);    
    
    state = digitalRead(PIN_HOME_BETA);
    digitalWrite(LED_B, state);
  }
}
