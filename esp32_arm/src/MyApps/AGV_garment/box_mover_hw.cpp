#include "box_mover_hw.h"


#define STEPS_PER_RAD_A 326   //2048 / 2*Pi
#define STEPS_PER_MM_Z 326   //2048 / 2*Pi
#define ACCELERATION_HOMIMG_A 2000
#define ACCELERATION_HOMIMG_Z 2000
#define MAX_SPEED_HOMING_Z 2000
#define MAX_SPEED_HOMING_A 2000

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

void BoxMoverHardware::HomeSingleAxis(char axis){
	Serial.print("[Debug] BoxMoverHardware::HomeSingleAxis() is entering\n" );
	Serial.print(axis);
	this->_homing_axis = axis;

	if (axis=='A'){
		this->objStepper_alpha.setAcceleration(ACCELERATION_HOMIMG_A);
		this->objStepper_alpha.setMaxSpeed(MAX_SPEED_HOMING_A);
		//todo :  process with IK()
		this->objStepper_alpha.setTargetRel(500000);
		this->objStepper_beta.setTargetRel(500000);
		this->__homing_helper = &this->objHomeHelper_angle;
	}else if (axis=='Z'){
		this->objStepper_beta.setAcceleration(ACCELERATION_HOMIMG_Z);
		this->objStepper_beta.setMaxSpeed(MAX_SPEED_HOMING_Z);
		this->objStepper_alpha.setTargetRel(500000);
		this->objStepper_beta.setTargetRel(-500000);	
		this->__homing_helper = &this->objHomeHelper_vertical;
	}
	this->objStepControl.moveAsync(this->objStepper_alpha, this->objStepper_beta);
}

void BoxMoverHardware::_running_G28(){
	// Serial.print("[Debug] BoxMoverHardware::running_G28() is entering \n");
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] Homed postion =  " );
		this->objStepControl.stop();
		// this->__homing_stepper->setPosition(0);
		// Serial.println(this->__homing_stepper->getPosition());
		this->State = IDLE;
		this->objStepper_alpha.setMaxSpeed(MAX_STEPS_PER_SECOND_ALPHA);
		this->objStepper_alpha.setAcceleration(MAX_ACCELERATION_ALPHPA);
		this->objStepper_beta.setMaxSpeed(MAX_STEPS_PER_SECOND_BETA);
		this->objStepper_beta.setAcceleration(MAX_ACCELERATION_BETA);
	}
}

void BoxMoverHardware::RunG1(Gcode* gcode) {
	//None blocking, move backgroundly.
	Serial.println("[Debug] BoxMoverHardware::RunG1()   \n");
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha.setMaxSpeed(speed);
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
    
    IkPosition_AB target_ik;
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
