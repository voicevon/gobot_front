#include "DCDriverHBridge.h"

DCDriverHBridge::DCDriverHBridge(int pinA, int pinB){
  // Pin initialization
  __pinA = pinA;
  __pinB = pinB;
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
}

void DCDriverHBridge::Start(int pwm_speed, bool dir_forward){
  if (dir_forward){
    digitalWrite(__pinA, HIGH);
    digitalWrite(__pinB, LOW);
  }else{
    digitalWrite(__pinA, LOW);
    digitalWrite(__pinB, HIGH);
  }
}
void DCDriverHBridge::Stop(){
    digitalWrite(__pinA, LOW);
    digitalWrite(__pinB, LOW);

}
void DCDriverHBridge::Break(){
    digitalWrite(__pinA, HIGH);
    digitalWrite(__pinB, HIGH);
}
