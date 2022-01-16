#include "box_mover_hw.h"


#define STEPS_PER_RAD 326   //2048 / 2*Pi
#define ACCELERATION_HOMIMG 2000
#define MAX_SPEED_HOMING 2000

#define MAX_STEPS_PER_SECOND_ALPHA 500
#define MAX_STEPS_PER_SECOND_BETA 500
#define MAX_ACCELERATION_ALPHPA 20
#define MAX_ACCELERATION_BETA 20

void BoxMoverHardware::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
    
}



BoxMoverHardware::BoxMoverHardware(){

}

void BoxMoverHardware::Init_Linkage(){
    
}

void BoxMoverHardware::RunG1(Gcode* gcode) {
	//None blocking, move backgroundly.
	Serial.println("[Debug] GobotHouseHardware::RunG1()   \n");
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha.setMaxSpeed(800);
	}

    FKPosition_ZA target_fk;
    target_fk.z = this->__current_fk_position.z;
    target_fk.a = this->__current_fk_position.a;

	if (gcode->has_letter('A')) 
        target_fk.a = gcode->get_value('A');
	if (gcode->has_letter('Z')) 
        target_fk.z = gcode->get_value('Z');

    Serial.print(this->__current_fk_position.z);
	Serial.print(",");
	Serial.println(this->__current_fk_position.a);
	Serial.print(" <-- from   FK(Z, alpha)   to --> ");
	Serial.print(target_fk.z);
	Serial.print(",");
	Serial.println(target_fk.a);
    
    IkPosAB target_ik;
    this->IK(&target_fk, &target_ik);

	this->objStepper_alpha.setTargetAbs(target_ik.alpha);
	this->objStepper_beta.setTargetAbs(target_ik.beta);
	this->objStepControl.moveAsync(this->objStepper_alpha, this->objStepper_beta);

}
void BoxMoverHardware::_running_G1(){
    if (this->GetDistanceToTarget_IK() < MAX_ACCELERATION_ALPHPA + MAX_ACCELERATION_BETA){
      	this->State = IDLE;
		Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);
}
