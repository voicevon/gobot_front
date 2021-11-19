#include "DCMotorController.h"

void DCMotorController::MoveForward(float speed, bool reverseDir){
    this->hBridge->MoveAtSpeed(1*speed,  reverseDir);
} 

