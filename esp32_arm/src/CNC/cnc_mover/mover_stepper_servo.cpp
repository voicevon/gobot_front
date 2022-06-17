#include "mover_stepper_servo.h"


void CncMover_StepperServo::LinkStepper_asAlpha(ActuatorStepper* stepper){
    this->__stepper_alpha = stepper;
}

void CncMover_StepperServo::LinkServo_asBeta(ActuatorServo* servo){
    this->__servo_beta = servo;
}

void CncMover_StepperServo::AllMotorsMoveTo(bool is_absolute_position, float* positions_in_cnc_unit, int motors_count){
    // float new_angle = motor_position[1];
    if (motors_count==2){
        // if (is_absolute_position){
        //     this->__stepper_alpha->setTargetAbs(motor_position[0]);
        // } else {
        //     this->__stepper_alpha->setTargetRel(motor_position[0]);
        //     // new_angle += this->__servo_beta->GetCurrentPosition();
        //     // this->__servo_beta->Write(new_angle);
        // }
        this->__stepper_alpha->MoveTo(is_absolute_position, positions_in_cnc_unit[0]);
        Stepper* alpha = this->__stepper_alpha->GetLinkedStepper();
        this->__stepControl.moveAsync(*alpha);
        this->__servo_beta->MoveTo(is_absolute_position, positions_in_cnc_unit[1]);
    }
}

void CncMover_StepperServo::AllMotorStop(){
    this->__stepControl.stop();
}

void CncMover_StepperServo::SingleMotorMoveTo(bool is_absolute_position, char motor_name, float position_in_cnc_unit){
    if (motor_name == 'A'){
        this->__stepper_alpha->MoveTo(is_absolute_position, position_in_cnc_unit);
        Stepper* stepper = this->__stepper_alpha->GetLinkedStepper();
        this->__stepControl.moveAsync(*stepper);
    }else if (motor_name == 'B'){
            this->__servo_beta->MoveTo(is_absolute_position, position_in_cnc_unit);
    }else{
        log_w("CncMover_StepperServo::SingleMotorMoveTo() axisname= ", motor_name );
    }
}

float CncMover_StepperServo::GetMotorPosition_InCncUnit(char motor_name){
    if (motor_name == 'A'){
        // return this->__stepper_alpha->getPosition();
        int32_t motor_position = this->__stepper_alpha->GetCurrentPosition_InCncUnit();
        // convert motor_position to cnc position
        float steps_per_unit = 12.34;
        float cnc_position = motor_position / steps_per_unit;
        return cnc_position;

    }else if (motor_name == 'B'){
        return this->__servo_beta->GetCurrentPosition_InCncUnit();
    }else{
        log_w("CncMover_StepperServo::SingleMotorMoveTo() axisname= ", motor_name );
    }
}

void CncMover_StepperServo::SetMotorPosition(char motor_name, float as_current_position){
    if (motor_name == 'A'){
        // this->__stepper_alpha->setPosition(as_current_position);
        this->__stepper_alpha->SetPosition(as_current_position);
    }else if (motor_name == 'B'){
        this->__servo_beta->SetPosition(as_current_position);
        // log_w("[]CncMover_StepperServo::SingleMotorMoveTo()  "," Not Implemented");
    }else{
        log_w("[Warn] CncMover_StepperServo::SingleMotorMoveTo() Unkonwn axisname= ", motor_name );
    }
}
float CncMover_StepperServo::GetDistanceToTarget_InCncUnit(){
    // return this->__stepper_alpha->getDistanceToTarget();
    float alpha_distance = this->__stepper_alpha->GetDistanceToTarget();
    float beta_distance = this->__servo_beta->GetDistanceToTarget();
    return sqrt(alpha_distance * alpha_distance + beta_distance * beta_distance);

}
