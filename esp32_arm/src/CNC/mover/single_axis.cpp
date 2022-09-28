#include "single_axis.h"
#include "MyLibs/calculator.h"

// void Mover_SingleAxis::SetEefSpeed(float speed){
//     if (this->_moving_actuator_flags == 0x01){
//         //speed is for alpha
//         this->_actuator_alpha_base->SetSpeed(speed);
//     }else{
//         Serial.print("[Warn] Mover_SingleAxis::SetSpeed() Unknown flag = ");
//         Serial.println(this->_moving_actuator_flags);
//     }
// }

// alpha = flags.bits[0]
void Mover_SingleAxis::AllActuatorsMoveTo(uint8_t is_absolute_position_flags, float* positions_in_cnc_unit){
    bool is_absolute_position;
    uint8_t target_motor_flags = this->_moving_actuator_flags;

    // Step1:  Set target motor position. determin absolute or relative.
    if (target_motor_flags == 0x01){
        // set alpha position
        is_absolute_position = (is_absolute_position_flags & 0x01) > 0;
        // this->_actuator_alpha_base->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[0]);
        MoveBlock_SingleActuator move;
        move.IsAbsTargetPosition = is_absolute_position;
        move.TargetPosition = positions_in_cnc_unit[0];
        // this->_actuator_alpha_base->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[0]);
        this->_actuator_alpha_base->UpdateMovement(&move);
    }else{
        Logger::Error("Mover_SingleAxis::AllActuatorsMoveTo()");
        Logger::Print("target_motor_flags", target_motor_flags);
        
    }

    //Step2:  move one or all motors.
    if (target_motor_flags == 0x01){
        // Move alpha only
        // this->__stepControl->moveAsync(*alpha);
        // this->_actuator_alpha_base->SpinOnce();
        
        bool debug = true;
        if(debug){
            Logger::Debug("Mover_SingleAxis::AllActuatorsMoveTo()");
            // Logger::Print("alpha_time", alpatim)
            // Serial.print(alpha_time_in_second);
            // Serial.print( "->" );
            // Serial.print (this->_actuator_alpha_base->GetNeededSeconds());
            // Serial.print("   Beta time= ");
            // Serial.print(beta_time_in_second);
            // Serial.print(" -> ");
            // Serial.println(this->__actuator_beta->GetNeededSeconds());
        }
        

    }else{
        Serial.print("[Error] Mover_SingleAxis::AllMotorsMoveTo()  target_motor_flags= ");
        Serial.println(target_motor_flags);
    }
    this->_moving_actuator_flags = target_motor_flags;
}

void Mover_SingleAxis::AllActuatorsStop(){
    this->_actuator_alpha_base->ForceStop();
    this->_actuator_alpha_base->UpdateTargetPositionFromCurrent();
    
}

void Mover_SingleAxis::SingleActuatorStop(EnumAxis actuator_name){
    this->_actuator_alpha_base->UpdateTargetPositionFromCurrent();
}


void Mover_SingleAxis::SingleActuatorMoveTo(MoveBlock_SingleActuator* line){
    // if (actuator_name == AXIS_ALPHA){
    if (line->axis == AXIS_ALPHA){
        this->_moving_actuator_flags = 0x01;
        this->_actuator_alpha_base->UpdateMovement(line);
        
    }else{
        log_w("Mover_SingleAxis::SingleMotorMoveTo() axisname= ", actuator_name );
    }
}


bool Mover_SingleAxis::ActuatorIsMoving(EnumAxis actuator_name) {
    if (actuator_name==AXIS_ALPHA){
        // return this->_actuator_alpha_base->IsMoving();

    }else{
        Serial.print("[Error] Mover_SingleAxis::MotorIsMoving() Unknown motor_name= ");
        Serial.println(actuator_name); 
    }
    return false;
}

bool Mover_SingleAxis::HasArrivedTargetPosition(){
    if (this->GetAbsDistanceToTarget_InCncUnit() < this->_small_distance_for_arriving_target_position)
        return true;
    return false;
}
