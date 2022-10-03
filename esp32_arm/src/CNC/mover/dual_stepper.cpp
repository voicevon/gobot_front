// #include "dual_stepper.h"

// // void Mover_DualStepper::Init(CncBoardBase* board){
// //     this->__actuator_alpha = board->GetActuator(AXIS_ALPHA);
// // }

// void Mover_DualStepper::LinkStepper_asAlpha(ActuatorStepper* alpha){
//     Logger::Debug("Mover_DualStepper::LinkStepper_asAlpha()");
//     alpha->SetCurrentPositionAs(0);
//     alpha->GetLinkedStepper()->setPosition(0);
//     this->__actuator_alpha = alpha;
// }

// void Mover_DualStepper::LinkStepper_asBeta(ActuatorStepper* beta){
//     beta->SetCurrentPositionAs(0);
//     beta->GetLinkedStepper()->setPosition(0);
//     this->__actuator_beta = beta;
// }
// #include "MyLibs/calculator.h"


// // void Mover_DualStepper::SetEefSpeed(float speed){
// //     //TODO:  set eef speed, not actuator speed
// //     if (this->_moving_actuator_flags == 0x01){
// //         //speed is for alpha
// //         this->__actuator_alpha->SetSpeed(speed);
// //     }else if (this->_moving_actuator_flags == 0x02){
// //         //spped is for beta
// //         this->__actuator_beta->SetSpeed(speed);
// //     }else if (this->_moving_actuator_flags == 0x03){
// //         //speed is for both.   TODO:  speed = alpha * alpha + beta * beta;
// //         this->__actuator_alpha->SetSpeed(speed);
// //         this->__actuator_beta->SetSpeed(speed);
// //     }else{
// //         Serial.print("[Warn] Mover_DualStepper::SetSpeed() Unknown flag = ");
// //         Serial.println(this->_moving_actuator_flags);
// //     }
// // }

// // alpha = flags.bits[0]
// // beta = flags.bits[1]
// // void Mover_DualStepper::AllActuatorsMoveTo(uint8_t is_absolute_position_flags, float* positions_in_cnc_unit){
// // void Mover_DualStepper::AllActuatorsMoveTo(MoveBlock* move){
//     //     bool is_absolute_position;
//     //     Stepper* alpha = this->__actuator_alpha->GetLinkedStepper();
//     //     Stepper* beta = this->__actuator_beta->GetLinkedStepper();

//     //     uint8_t target_motor_flags = this->_moving_actuator_flags;
//     //     MoveBlock_SingleActuator move;
//     //     // Step1:  Set target motor position. determin absolute or relative.
//     //     if (target_motor_flags == 0x01){
//     //         // set alpha position
//     //         is_absolute_position = (is_absolute_position_flags & 0x01) > 0;
//     //         // this->__actuator_alpha->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[0]);
//     //         move.IsAbsTargetPosition= is_absolute_position;
//     //         move.TargetPosition = positions_in_cnc_unit[0];
//     //         this->__actuator_alpha->UpdateMovement(&move);

//     //     }else if (target_motor_flags == 0x02){
//     //         // set beta position
//     //         is_absolute_position = (is_absolute_position_flags & 0x02) > 0;
//     //         // this->__actuator_beta->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[1]);
//     //         move.IsAbsTargetPosition = is_absolute_position;
//     //         move.TargetPosition = positions_in_cnc_unit[1];
//     //         this->__actuator_beta->UpdateMovement(&move);

//     //     }else if (target_motor_flags == 0x03){
//     //         // set alpha position
//     //         is_absolute_position = (is_absolute_position_flags & 0x01) > 0;
//     //         // this->__actuator_alpha->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[0]);
//     //         move.IsAbsTargetPosition = is_absolute_position;
//     //         move.TargetPosition = positions_in_cnc_unit[0];
//     //         this->__actuator_alpha->UpdateMovement(&move);
//     //         // and set beta position
//     //         is_absolute_position = (is_absolute_position_flags & 0x02) > 0;
//     //         // this->__actuator_beta->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[1]);
//     //         move.IsAbsTargetPosition = is_absolute_position;
//     //         move.TargetPosition = positions_in_cnc_unit[1];
//     //         this->__actuator_beta->UpdateMovement(&move);
//     //     }

//     //     //Step2:  move one or all motors.
//     //     if (target_motor_flags == 0x01){
//     //         // Move alpha only
//     //         this->__stepControl->moveAsync(*alpha);

//     //     }else if (target_motor_flags == 0x02){
//     //         // move beta only
//     //         // this->__actuator_beta->StartToMove();
//     //         this->__stepControl->moveAsync(*beta);

//     //     }else if (target_motor_flags == 0x03){
//     //         // Both motor will move
//     //         this->__stepControl->moveAsync(*alpha, *beta);
//     //         // this->__actuator_beta->StartToMove();

//     //     }else{
//     //         Logger::Error("Mover_DualStepper::AllMotorsMoveTo()");
//     //         Logger::Print("target_motor_flags", target_motor_flags);
//     //     }
//     //     this->_moving_actuator_flags = target_motor_flags;
//     // }

//     // void Mover_DualStepper::AllActuatorsStop(){
//     //     this->__stepControl->stop();
//     //     this->__actuator_beta->UpdateTargetPositionFromCurrent();
// // }

// // void Mover_DualStepper::SingleActuatorStop(EnumAxis actuator_name){
// //     if (actuator_name == AXIS_ALPHA){
// //         this->__actuator_alpha->UpdateTargetPositionFromCurrent();
// //         this->_moving_actuator_flags -= 0x01;

// //     }else if (actuator_name==AXIS_BETA){
// //         this->__actuator_beta->UpdateTargetPositionFromCurrent();
// //         this->_moving_actuator_flags -= 0x02;
// //     }else{
// //         Serial.print("[Error] Mover_DualStepper::SingleMotorStop() Unknown actuator_name= ");
// //         Serial.println(actuator_name);
// //     }
// // }


// // void Mover_DualStepper::SingleActuatorMoveTo(EnumAxis actuator_name, bool is_absolute_position, float position_in_cnc_unit){
// // void Mover_DualStepper::SingleActuatorMoveTo(LineSegment* move){
// // void Mover_DualStepper::SingleActuatorMoveTo(MoveBlock_SingleActuator* move){
// //     Stepper* stepper;
// //     // if (actuator_name == AXIS_ALPHA){
// //     if (move->axis == AXIS_ALPHA){
// //         // this->__actuator_alpha->SetTargetPositionTo(is_absolute_position, position_in_cnc_unit);
// //         // this->__actuator_alpha->SetTargetPositionTo(move->IsAbsTargetPosition, move->TargetPosition);
// //         this->__actuator_alpha->UpdateMovement(move);
// //         stepper = this->__actuator_alpha->GetLinkedStepper();
// //         // this->__stepControl->moveAsync(*stepper);
// //         this->_moving_actuator_flags = 0x01;

// //     // }else if (actuator_name == AXIS_BETA){
// //     }else if (move->axis == AXIS_BETA){
// //         // this->__actuator_beta->SetTargetPositionTo(is_absolute_position, position_in_cnc_unit);
// //         // this->__actuator_beta->SetTargetPositionTo(move->IsAbsTargetPosition, move->TargetPosition);
// //         // this->__actuator_beta->SetTargetPositionTo(move);
// //         this->__actuator_beta->UpdateMovement(move);

// //         stepper = this->__actuator_beta->GetLinkedStepper();
// //         // this->__stepControl->moveAsync(*stepper);
// //         this->_moving_actuator_flags = 0x02;

// //     }else{
// //         Logger::Halt ("Mover_DualStepper::SingleMotorMoveTo() axisname= " );

// //     }
// //     this->__stepControl->moveAsync(*stepper);

// // }





// // float Mover_DualStepper::GetAbsDistanceToTarget_InCncUnit(){
// //     float alpha_distance = 0;
// //     if((this->_moving_actuator_flags & 0x01) > 0){
// //        alpha_distance = this->__actuator_alpha->GetAbsDistanceToTarget_InCncUnit();
// //     }
// //     float beta_distance = 0;
// //     if ((this->_moving_actuator_flags & 0x02) > 0){
// //        beta_distance = this->__actuator_beta->GetAbsDistanceToTarget_InCncUnit();
// //     }
// //     bool debug= false;
// //     if(debug){
// //         Serial.print("[Debug] Mover_DualStepper::GetAbsDistanceToTarget_InCncUnit() alpha = ");
// //         Serial.print(alpha_distance);
// //         Serial.print("  beta = ");
// //         Serial.println(beta_distance);
// //     }
// //     return sqrt(alpha_distance * alpha_distance + beta_distance * beta_distance);
// // }

// // void Mover_DualStepper::SetActuatorSpeed(EnumAxis actuator_name, float steps_per_second){
// //     if (actuator_name ==AXIS_ALPHA){
// //         this->__actuator_alpha->SetSpeed(steps_per_second);
// //     }else if(actuator_name == AXIS_BETA){
// //         this->__actuator_beta->SetSpeed(steps_per_second);
// //     }else{
// //         Serial.println("[Error] Mover_DualStepper::SetActuatorSpeed() ");
// //     }
// // }



// bool Mover_DualStepper::ActuatorIsMoving(EnumAxis_Inverseinematic actuator_name) {
//     if (actuator_name==AXIS_ALPHA){
//         return this->__stepControl->isRunning();
//     }else if (actuator_name==AXIS_BETA){
//         return this->__stepControl->isRunning();
//     }else{
//         Serial.print("[Error] Mover_DualStepper::MotorIsMoving() Unknown motor_name= ");
//         Serial.println(actuator_name); 
//     }
//     return false;
// }

// // void Mover_DualStepper::PrintOut(const char* title){
// //     Logger::Info(title);
// //     Serial.print("moving_flags= ");
// //     Serial.println(this->_moving_actuator_flags);
    
// //     if ((this->_moving_actuator_flags & 0x01) == 1){
// //         Serial.print("For alpha:  ");
// //         Serial.print("  current_position= ");
// //         Serial.print(this->__actuator_alpha->GetCurrentCncPosition());
// //         Serial.print("  target_position= ");
// //         Serial.print(this->__actuator_alpha->GetTartetCncPosition());
// //         Serial.println();
// //     }
// //     if ((this->_moving_actuator_flags & 0x02) == 2){
// //         Serial.print("For beta:  target_position= ");
// //         Serial.print(this->__actuator_alpha->GetTartetCncPosition());
// //         Serial.println();
// //     }

// //     Serial.println(FCBC_RESET);
// // }
