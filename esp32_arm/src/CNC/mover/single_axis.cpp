// #include "single_dc_motor.h"
#include "single_axis.h"

void CncMover_SingleAxis::PrintOut(const char* title) {
    Logger::Info("CncMover_SingleAxis::PrintOut eeeeeeeeeeee");
}

#include "MyLibs/calculator.h"

void CncMover_SingleAxis::SetEefSpeed(float speed){
    if (this->_moving_actuator_flags == 0x01){
        //speed is for alpha
        this->_actuator_alpha->SetSpeed(speed);
    }else{
        Serial.print("[Warn] CncMover_SingleAxis::SetSpeed() Unknown flag = ");
        Serial.println(this->_moving_actuator_flags);
    }
}

// alpha = flags.bits[0]
void CncMover_SingleAxis::AllActuatorsMoveTo(uint8_t is_absolute_position_flags, float* positions_in_cnc_unit){
    bool is_absolute_position;
    uint8_t target_motor_flags = this->_moving_actuator_flags;

    // Step1:  Set target motor position. determin absolute or relative.
    if (target_motor_flags == 0x01){
        // set alpha position
        is_absolute_position = (is_absolute_position_flags & 0x01) > 0;
        this->_actuator_alpha->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[0]);
    }else{
        Serial.print("[Error]() CncMover_SingleAxis::AllActuatorsMoveTo()  target_motor_flags= ");
        Serial.println(target_motor_flags);
        while (true){
            Serial.print("E ");
            delay(1000);
        }
        
    }

    //Step2:  move one or all motors.
    if (target_motor_flags == 0x01){
        // Move alpha only
        // this->__stepControl->moveAsync(*alpha);
        // this->_actuator_alpha->SpinOnce();
        
        bool debug = true;
        if(debug){
            Serial.print("[Debug] CncMover_SingleAxis::AllActuatorsMoveTo()  alpha_time= ");
            // Serial.print(alpha_time_in_second);
            // Serial.print( "->" );
            // Serial.print (this->_actuator_alpha->GetNeededSeconds());
            // Serial.print("   Beta time= ");
            // Serial.print(beta_time_in_second);
            // Serial.print(" -> ");
            // Serial.println(this->__actuator_beta->GetNeededSeconds());
        }
        

    }else{
        Serial.print("[Error] CncMover_SingleAxis::AllMotorsMoveTo()  target_motor_flags= ");
        Serial.println(target_motor_flags);
    }
    this->_moving_actuator_flags = target_motor_flags;
}

void CncMover_SingleAxis::AllActuatorsStop(){
    this->_actuator_alpha->Stop();
}

void CncMover_SingleAxis::SingleActuatorStop(EnumAxis actuator_name){
    this->_actuator_alpha->Stop();
}


void CncMover_SingleAxis::SingleActuatorMoveTo(EnumAxis actuator_name, bool is_absolute_position, float position_in_cnc_unit){
    if (actuator_name == AXIS_ALPHA){
        this->_moving_actuator_flags = 0x01;
        this->_actuator_alpha->SetTargetPositionTo(is_absolute_position, position_in_cnc_unit);
    }else{
        log_w("CncMover_SingleAxis::SingleMotorMoveTo() axisname= ", actuator_name );
    }
}

float CncMover_SingleAxis::GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis actuator_name){
    if (actuator_name == AXIS_ALPHA){
        return this->_actuator_alpha->GetCurrentPosition_InCncUnit();
    }else{
        log_w("CncMover_SingleAxis::SingleMotorMoveTo() axisname= ", actuator_name );
    }
    return 0;
}

void CncMover_SingleAxis::SetActuatorCurrentCncPositionAs(EnumAxis actuator_name, float as_current_position){
    if (actuator_name == AXIS_ALPHA){
        this->_actuator_alpha->SetCurrentPositionAs(as_current_position);
    }else{
        log_w("[Warn] CncMover_SingleAxis::SingleMotorMoveTo() Unkonwn axisname= ", actuator_name );
    }
}
float CncMover_SingleAxis::GetAbsDistanceToTarget_InCncUnit(){
    float alpha_distance = 0;
    if((this->_moving_actuator_flags & 0x01) > 0){
       alpha_distance = this->_actuator_alpha->GetAbsDistanceToTarget_InCncUnit();
    }
    bool debug= false;
    if(debug){
        Serial.print("[Debug] CncMover_SingleAxis::GetAbsDistanceToTarget_InCncUnit() alpha = ");
        Serial.print(alpha_distance);
    }
    return sqrt(alpha_distance * alpha_distance);

}

// void CncMover_SingleAxis::SetActuatorSpeed(EnumAxis actuator_name, float speed_per_second){
    
//     if (actuator_name == AXIS_ALPHA){
//         this->_actuator_alpha->SetSpeed(speed_per_second);
//     }else{
//         Serial.println("[Error]() CncMover_SingleAxis::SetActuatorSpeed");
//     }
// }

bool CncMover_SingleAxis::ActuatorIsMoving(EnumAxis actuator_name) {
    if (actuator_name==AXIS_ALPHA){
        // return this->_actuator_alpha->IsMoving();

    }else{
        Serial.print("[Error] CncMover_SingleAxis::MotorIsMoving() Unknown motor_name= ");
        Serial.println(actuator_name); 
    }
    return false;
}
