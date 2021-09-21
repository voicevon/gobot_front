
#include "SingleAxis.h"


// 
// SingleAxis::SingleAxis(char axis_name){
//   this->_Axis_Name = axis_name;
//   // this->_is_homing = false;
// }


// 
// void SingleAxis::SetTargetAbs(int targetPosition){

// }


void SingleAxis::Init_scaler(float _final_distance_per_encoder_interval){
  final_distance_per_encoder_interval = _final_distance_per_encoder_interval;
}


void SingleAxis::SpinOnce(){
  this->__is_busy = false;
  if (_actuator->IsRunning) this->__is_busy = true;
  if (__is_homing) this->__is_busy = true;
}


// void SingleAxis::LinkAcuator(ActuatorBase* actuator){
//   this->_actuator = actuator;
// }

// 
// void SingleAxis::LinkHomeTriger(HomeTriger* homeTriger){
//   this->homeTriger = homeTriger;
// }


// 
// void SingleAxis::Move(float distanceRel){

// }
// 
// void SingleAxis::MoveAsync(){

// }
// 
// void SingleAxis::Home(){
//   while (!homeTriger->IsTriged()){
//     // actuator->SetPositionRel(100);
//     actuator->setTargetAbs(100);
//     actuator->Move();
//   }  
// }


// void SingleAxis::Setup(void (*ISR_callback)(void)){
//     attachInterrupt(digitalPinToInterrupt(__pinA), ISR_callback, RISING);
//     // attachInterrupt(digitalPinToInterrupt(3),&SingleAxis::ISR_Encoder, 0);
//     attachInterrupt(5,  [this](void)ICACHE_RAM_ATTR{ encoderPos++; },RISING); 
//     attachInterrupt(5,  [this](void)ICACHE_FLASH_ATTR{ encoderPos++; },RISING); 
//     //https://github.com/esp8266/Arduino/issues/7788#issuecomment-750364982
//     // attachInterrupt(6,this->ISR_Encoder,FALLING );
// }
// void SingleAxis::setup2(std::function<void(void)> callback){
//     attachInterrupt(digitalPinToInterrupt(__pinA), callback, RISING);

// }
// void(*ISR_callback)();


// void ICACHE_RAM_ATTR SingleAxis::ISR_Encoder(void){
//   cli(); //stop interrupts happening before we read pin values
//   if(digitalRead(__pinB)) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
//     encoderPos--; //decrement the encoder's position count
//   }
//   else
//     encoderPos++;
//   sei(); //restart interrupts
// }


