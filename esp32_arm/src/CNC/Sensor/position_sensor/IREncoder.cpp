#include "IrEncoder.h"


IrEncoder::IrEncoder(uint8_t _encA, uint8_t _encB , uint16_t _ppr, uint8_t _index){
  // this->raw_value_is_absolute = false;
  pinA = _encA;
  pinB = _encB;
  pulse_counter = 0;
  pulse_timestamp = 0;

  cpr = _ppr;
  this->pinIndex = _index; // its 0 if not used

  // velocity calculation variables
  prev_Th = 0;
  pulse_per_second = 0;
  prev_pulse_counter = 0;
  prev_timestamp_us = _micros();

  // extern pullup as default
  pullup = Pullup::USE_EXTERN;
}

//  Interrupt is multi fired!
//    https://github.com/espressif/esp-idf/issues/4180
//    https://www.esp32.com/viewtopic.php?t=3608
//    https://www.esp32.com/viewtopic.php?t=18736#p69578       NO Schmitt Trigger on GPIO!

void IrEncoder::handleB() {
  bool A = digitalRead(pinA);
  bool B = digitalRead(pinB);
  if (last_state_B == B){
    // Serial.print("e");
    return;
  }
  pulse_timestamp = _micros();
  if (A == B) {
    pulse_counter++;
    // Serial.print("+");
  }else{
    pulse_counter--;
    // Serial.print("-");
  }
  last_state_B = B;

}

void IrEncoder::handleIndex() {
  // if(hasIndex()){
  //   bool I = digitalRead(pinIndex);
  //   if(I && !I_active){
  //     index_found = true;
  //     // align encoder on each index
  //     long tmp = pulse_counter;
  //     // corrent the counter value
  //     pulse_counter = round((double)pulse_counter/(double)cpr)*cpr;
  //     // preserve relative speed
  //     prev_pulse_counter += pulse_counter - tmp;
  //   }
  //   I_active = I;
  // }
}

/*
	Shaft angle calculation
*/
float IrEncoder::getAngle(){
  // Serial.println(pulse_counter);
  // Serial.println(cpr);
  return  _2PI * (pulse_counter) / ((float)cpr);
}
/*
  Shaft velocity calculation
  function using mixed time and frequency measurement technique
*/
float IrEncoder::getVelocity(){
  // timestamp
  long timestamp_us = _micros();
  // sampling time calculation
  float Ts = (timestamp_us - prev_timestamp_us) * 1e-6;
  // quick fix for strange cases (micros overflow)
  if(Ts <= 0 || Ts > 0.5) Ts = 1e-3;

  // time from last impulse
  float Th = (timestamp_us - pulse_timestamp) * 1e-6;
  long dN = pulse_counter - prev_pulse_counter;

  // Pulse per second calculation (Eq.3.)
  // dN - impulses received
  // Ts - sampling time - time in between function calls
  // Th - time from last impulse
  // Th_1 - time form last impulse of the previous call
  // only increment if some impulses received
  float dt = Ts + prev_Th - Th;
  pulse_per_second = (dN != 0 && dt > Ts/2) ? dN / dt : pulse_per_second;

  // if more than 0.05 passed in between impulses
  if ( Th > 0.1) pulse_per_second = 0;

  // velocity calculation
  float velocity = pulse_per_second / ((float)cpr) * (_2PI);

  // save variables for next pass
  prev_timestamp_us = timestamp_us;
  // save velocity calculation variables
  prev_Th = Th;
  prev_pulse_counter = pulse_counter;
  return velocity;
}

// getter for index pin
// return -1 if no index
int IrEncoder::needsSearch(){
  return hasIndex() && !index_found;
}

// private function used to determine if encoder has index
int IrEncoder::hasIndex(){
  return pinIndex != 0;
}


// encoder initialisation of the hardware pins
// and calculation variables
void IrEncoder::init(){

  // Encoder - check if pullup needed for your encoder
  if(pullup == Pullup::USE_INTERN){
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    if(hasIndex()) pinMode(pinIndex,INPUT_PULLUP);
  }else{
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    if(hasIndex()) pinMode(pinIndex,INPUT);
  }

  // counter setup
  pulse_counter = 0;
  pulse_timestamp = _micros();
  // velocity calculation variables
  prev_Th = 0;
  pulse_per_second = 0;
  prev_pulse_counter = 0;
  prev_timestamp_us = _micros();
}

// function enabling hardware interrupts of the for the callback provided
// if callback is not provided then the interrupt is not enabled
void IrEncoder::enableInterrupts(void(*doB)(), void(*doIndex)()){
  if(doB != nullptr) 
    attachInterrupt(digitalPinToInterrupt(pinB), doB, CHANGE);

  // if index used initialize the index interrupt
  if(hasIndex() && doIndex != nullptr) 
    attachInterrupt(digitalPinToInterrupt(pinIndex), doIndex, CHANGE);
}
