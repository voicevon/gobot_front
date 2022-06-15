#include "actuator_servo.h"

float ActuatorServo::Read(){}{
    return this->__servo.read();
}

void ActuatorServo::Write(float angle){

}