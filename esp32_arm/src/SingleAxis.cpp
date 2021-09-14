#include "SingleAxis.h"

#define PIN_HOME_SENSOR 5
#define PIN_ENCODER_A 6
#define PIN_ENCODER_B 7
#define PIN_DC_MOTOR_A 8
#define PIN_DC_MOTOR_B 9



void SingleAxis::SetTargetAbs(int targetPosition){

}


SingleAxis::SingleAxis(uint8_t pinA, uint8_t pinB, uint8_t pin_home_sensor){
    __pinA = pinA;
    __pinB = pinB;
    __pinHomeSensor = pin_home_sensor;
    pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
    pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
}

void SingleAxis::Setup(void (*ISR_callback)(void)){
    attachInterrupt(digitalPinToInterrupt(__pinA), ISR_callback, RISING);
    // attachInterrupt(digitalPinToInterrupt(3),&SingleAxis::ISR_Encoder, 0);
    attachInterrupt(5,  [this](void)ICACHE_RAM_ATTR{ encoderPos++; },RISING); 
    attachInterrupt(5,  [this](void)ICACHE_FLASH_ATTR{ encoderPos++; },RISING); 
    //https://github.com/esp8266/Arduino/issues/7788#issuecomment-750364982
    // attachInterrupt(6,this->ISR_Encoder,FALLING );
}
void SingleAxis::setup2(std::function<void(void)> callback){
    attachInterrupt(digitalPinToInterrupt(__pinA), callback, RISING);

}
void(*ISR_callback)();


void ICACHE_RAM_ATTR SingleAxis::ISR_Encoder(void){
  cli(); //stop interrupts happening before we read pin values
  if(digitalRead(__pinB)) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos--; //decrement the encoder's position count
  }
  else
    encoderPos++;
  sei(); //restart interrupts
}