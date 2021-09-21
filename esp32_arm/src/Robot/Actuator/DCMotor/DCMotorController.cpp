#include "DCMotorController.h"

void DCMotorController::MoveForward(float speed, bool reverseDir){
    this->hBridge->Start(1*speed,  reverseDir);
} 

