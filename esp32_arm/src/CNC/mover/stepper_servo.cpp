#include "stepper_servo.h"


void CncMover_StepperServo::LinkStepper_asAlpha(ActuatorStepper* alpha){
    alpha->SetCurrentPositionAs(0);
    alpha->GetLinkedStepper()->setPosition(0);
    this->__actuator_alpha = alpha;
}

void CncMover_StepperServo::LinkServo_asBeta(ActuatorServo* servo){
    this->__actuator_beta = servo;
}

// void CncMover_StepperServo::PrintOut(const char* title) {
//     Logger::Info("CncMover_StepperServo::PrintOut eeeeeeeeeeee");

// }

#include "MyLibs/calculator.h"

// void CncMover_StepperServo::SetEefSpeed(float speed){
//     if (this->_moving_actuator_flags == 0x01){
//         //speed is for alpha
//         this->__actuator_alpha->SetSpeed(speed);
//     }else if (this->_moving_actuator_flags == 0x02){
//         //spped is for beta
//         this->__actuator_beta->SetSpeed(speed);
//     }else if (this->_moving_actuator_flags == 0x03){
//         //speed is for both.   TODO:  speed = alpha * alpha + beta * beta;
//         this->__actuator_alpha->SetSpeed(speed);
//         this->__actuator_beta->SetSpeed(speed);
//     }else{
//         Serial.print("[Warn] CncMover_StepperServo::SetSpeed() Unknown flag = ");
//         Serial.println(this->_moving_actuator_flags);
//     }
// }

// alpha = flags.bits[0]
// beta = flags.bits[1]
// void CncMover_StepperServo::AllActuatorsMoveTo(uint8_t is_absolute_position_flags, float* positions_in_cnc_unit){
void CncMover_StepperServo::AllActuatorsMoveTo(MoveBlock* move){
    // bool is_absolute_position;
    // Stepper* alpha = this->__actuator_alpha->GetLinkedStepper();
    // uint8_t target_motor_flags = this->_moving_actuator_flags;
    // // MoveBlock_SingleActuator move;

    // this->_actuator_alpha_base->SetAccelleration()

    // // Step1:  Set target motor position. determin absolute or relative.
    // if (target_motor_flags == 0x01){
    //     // set alpha position
    //     // is_absolute_position = (is_absolute_position_flags & 0x01) > 0;
    //     // this->__actuator_alpha->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[0]);
    //     move.IsAbsTargetPosition = (is_absolute_position_flags & 0x01) > 0;
    //     move.TargetPosition = positions_in_cnc_unit[0];
    //     this->__actuator_alpha->UpdateMovement(&move);

    // }else if (target_motor_flags == 0x02){
    //     // set beta position
    //     // is_absolute_position = (is_absolute_position_flags & 0x02) > 0;
    //     // this->__actuator_beta->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[1]);
    //     move.IsAbsTargetPosition = (is_absolute_position_flags & 0x02) > 0;
    //     move.TargetPosition = positions_in_cnc_unit[1];
    //     this->__actuator_beta->UpdateMovement(&move);

    // }else if (target_motor_flags == 0x03){
    //     // set alpha position
    //     // is_absolute_position = (is_absolute_position_flags & 0x01) > 0;
    //     // this->__actuator_alpha->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[0]);
    //     move.IsAbsTargetPosition = (is_absolute_position_flags & 0x01) > 0;
    //     move.TargetPosition = positions_in_cnc_unit[0];
    //     this->__actuator_alpha->UpdateMovement(&move);
    //     // and set beta position
    //     // is_absolute_position = (is_absolute_position_flags & 0x02) > 0;
    //     // this->__actuator_beta->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[1]);
    //     move.IsAbsTargetPosition = (is_absolute_position_flags & 0x02) > 0;
    //     move.TargetPosition = positions_in_cnc_unit[1];
    //     this->__actuator_beta->UpdateMovement(&move);

    // }else{
    //     Serial.print("[Error]() CncMover_StepperServo::AllActuatorsMoveTo()  target_motor_flags= ");
    //     Serial.println(target_motor_flags);
    //     while (true){
    //         Serial.print("E ");
    //         delay(1000);
    //     }
        
    // }

    // //Step2:  move one or all motors.
    // if (target_motor_flags == 0x01){
    //     // Move alpha only
    //     this->__stepControl->moveAsync(*alpha);
    // }else if (target_motor_flags == 0x02){
    //     // move beta only
    //     this->__actuator_beta->StartToMove();

    // }else if (target_motor_flags == 0x03){
    //     // Both motors will move. Should end at the same time. so, one of the motor should be slower.
    //     float alpha_time_in_second = this->__actuator_alpha->GetNeededSeconds() + 1.8;  //TODO:  consider accleration
    //     float beta_time_in_second = this->__actuator_beta->GetNeededSeconds();
    //     if (beta_time_in_second < alpha_time_in_second){
    //         // beta should be slower, saying renew its speed.
    //         this->__actuator_beta->RenewSpeed(alpha_time_in_second);
    //     }else{
    //         // alpha should be slower, saying renew its speed.
    //         this->__actuator_alpha->RenewSpeed(beta_time_in_second);
    //         // below line is because the bug of Teensy-step.
    //         // this->__actuator_alpha->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[0]);   //Must redo set position after setting speed.
    //         move.IsAbsTargetPosition = is_absolute_position;
    //         move.TargetPosition =  positions_in_cnc_unit[0];
    //         this->__actuator_alpha->UpdateMovement(&move);   //Must redo set position after setting speed.
    //     }
        
    //     bool debug = true;
    //     if(debug){
    //         Serial.print("[Debug] CncMover_StepperServo::AllActuatorsMoveTo()  alpha_time= ");
    //         Serial.print(alpha_time_in_second);
    //         Serial.print( "->" );
    //         Serial.print (this->__actuator_alpha->GetNeededSeconds());
    //         Serial.print("   Beta time= ");
    //         Serial.print(beta_time_in_second);
    //         Serial.print(" -> ");
    //         Serial.println(this->__actuator_beta->GetNeededSeconds());
    //     }
        
    //     this->__stepControl->moveAsync(*alpha);
    //     this->__actuator_beta->StartToMove();

    // }else{
    //     Serial.print("[Error] CncMover_StepperServo::AllMotorsMoveTo()  target_motor_flags= ");
    //     Serial.println(target_motor_flags);
    // }
    // this->_moving_actuator_flags = target_motor_flags;
}

void CncMover_StepperServo::AllActuatorsStop(){
    this->__stepControl->stop();
    this->__actuator_beta->UpdateTargetPositionFromCurrent();
}

// void CncMover_StepperServo::SingleActuatorStop(EnumAxis actuator_name){
//     if (actuator_name == AXIS_ALPHA){
//         this->__actuator_alpha->UpdateTargetPositionFromCurrent();
//         this->_moving_actuator_flags -= 0x01;

//     }else if (actuator_name=='B'){
//         this->__actuator_beta->UpdateTargetPositionFromCurrent();
//         this->_moving_actuator_flags -= 0x02;
//     }else{
//         Serial.print("[Error] CncMover_StepperServo::SingleMotorStop() Unknown actuator_name= ");
//         Serial.println(actuator_name);
//     }
// }


// void CncMover_StepperServo::SingleActuatorMoveTo(MoveBlock_SingleActuator* move){
//     // if (actuator_name == AXIS_ALPHA){
//     if (move->axis == AXIS_ALPHA){
//         this->_moving_actuator_flags = 0x01;
//         // this->__actuator_alpha->SetTargetPositionTo(is_absolute_position, position_in_cnc_unit);
//         // this->__actuator_alpha->SetTargetPositionTo(move->IsAbsTargetPosition, move->TargetPosition);
//         // this->__actuator_alpha->SetTargetPositionTo(move);
//         this->__actuator_alpha->UpdateMovement(move);
//         Stepper* stepper = this->__actuator_alpha->GetLinkedStepper();
//         this->__stepControl->moveAsync(*stepper);

//     // }else if (actuator_name == 'B'){
//     }else if (move->axis == 'B'){
//         this->_moving_actuator_flags = 0x02;
//         // this->__actuator_beta->SetTargetPositionTo(is_absolute_position, position_in_cnc_unit);
//         // this->__actuator_beta->SetTargetPositionTo(move->IsAbsTargetPosition, move->TargetPosition);
//         // this->__actuator_beta->SetTargetPositionTo(move);
//         this->__actuator_beta->UpdateMovement(move);
//         this->__actuator_beta->StartToMove();

//     }else{
//         log_w("CncMover_StepperServo::SingleMotorMoveTo() axisname= ", actuator_name );
//     }
// }

// float CncMover_StepperServo::GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis actuator_name){
//     if (actuator_name == AXIS_ALPHA){
//         return this->__actuator_alpha->GetCurrentPosition();

//     }else if (actuator_name == 'B'){
//         return this->__actuator_beta->GetCurrentPosition();

//     }else{
//         log_w("CncMover_StepperServo::SingleMotorMoveTo() axisname= ", actuator_name );
//     }
//     return 0;
// }

// void CncMover_StepperServo::SetActuatorCurrentCncPositionAs(EnumAxis actuator_name, float as_current_position){
//     if (actuator_name == AXIS_ALPHA){
//         this->__actuator_alpha->SetCurrentPositionAs(as_current_position);
//     }else if (actuator_name == 'B'){
//         this->__actuator_beta->SetCurrentPositionAs(as_current_position);
//     }else{
//         log_w("[Warn] CncMover_StepperServo::SingleMotorMoveTo() Unkonwn axisname= ", actuator_name );
//     }
// }
// float CncMover_StepperServo::GetAbsDistanceToTarget_InCncUnit(){
//     float alpha_distance = 0;
//     if((this->_moving_actuator_flags & 0x01) > 0){
//        alpha_distance = this->__actuator_alpha->GetAbsDistanceToTarget_InCncUnit();
//     }
//     float beta_distance = 0;
//     if ((this->_moving_actuator_flags & 0x02) > 0){
//        beta_distance = this->__actuator_beta->GetAbsDistanceToTarget_InCncUnit();
//     }
//     bool debug= false;
//     if(debug){
//         Serial.print("[Debug] CncMover_StepperServo::GetAbsDistanceToTarget_InCncUnit() alpha = ");
//         Serial.print(alpha_distance);
//         Serial.print("  beta = ");
//         Serial.println(beta_distance);
//     }
//     return sqrt(alpha_distance * alpha_distance + beta_distance * beta_distance);

// }

// void CncMover_StepperServo::SetActuatorSpeed(EnumAxis actuator_name, float speed_per_second){
    
//     if (actuator_name == AXIS_ALPHA){
//         this->__actuator_alpha->SetSpeed(speed_per_second);
//     }else if(actuator_name == 'B'){
//         this->__actuator_beta->SetSpeed(speed_per_second);
//     }else{
//         Serial.println("[Error]() CncMover_StepperServo::SetActuatorSpeed");
//     }
// }

bool CncMover_StepperServo::ActuatorIsMoving(EnumAxis_Inverseinematic actuator_name) {
    if (actuator_name==AXIS_ALPHA){
        return this->__stepControl->isRunning();
    }else if (actuator_name=='B'){
        return this->__actuator_beta->IsMoving();
    }else{
        Serial.print("[Error] CncMover_StepperServo::MotorIsMoving() Unknown motor_name= ");
        Serial.println(actuator_name); 
    }
    return false;
}
