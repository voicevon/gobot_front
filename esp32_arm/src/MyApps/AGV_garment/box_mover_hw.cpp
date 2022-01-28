#include "box_mover_hw.h"


#define STEPS_PER_RAD 326   //2048 / 2*Pi
#define STEPS_PER_MM 326   //2048 / 2*Pi
#define ACCELERATION_HOMIMG_A 2000
#define ACCELERATION_HOMIMG_Z 2000
#define MAX_SPEED_HOMING_Z 2000
#define MAX_SPEED_HOMING_A 2000

#define MAX_STEPS_PER_SECOND_ALPHA 500
#define MAX_STEPS_PER_SECOND_BETA 500
#define MAX_ACCELERATION_ALPHPA 20
#define MAX_ACCELERATION_BETA 20


void BoxMoverHardware::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	FkPosition_ZW* fk = (FkPosition_ZW*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

	ik->alpha = 2 * (fk->Z * STEPS_PER_MM + fk->W * STEPS_PER_RAD);
	ik->beta = 2 * (fk->Z * STEPS_PER_MM -fk->W * STEPS_PER_RAD);
}

void BoxMoverHardware::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	FkPosition_ZW* fk = (FkPosition_ZW*)(to_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	
	fk->Z = (ik->alpha - ik->beta) / 2 / STEPS_PER_MM;
	fk->W = (ik->alpha + ik->beta) / 2 / STEPS_PER_RAD;
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

	Serial.print("\n[Debug] BoxMoverHardware::RunG1()   ");
	Serial.print(gcode->get_command());
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha.setMaxSpeed(speed);
		this->objStepper_beta.setMaxSpeed(speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_ZW target_fk_zw;
	IkPosition_AB target_ik_ab;
	target_fk_zw.Z = this->__current_fk_position.Z;
	target_fk_zw.W = this->__current_fk_position.W;
	target_ik_ab.alpha = float(this->objStepper_alpha.getPosition()) ;
	target_ik_ab.beta = float(this->objStepper_beta.getPosition());
	bool do_ik=false;
	if (gcode->has_letter('A')) target_ik_ab.alpha = gcode->get_value('A');
	if (gcode->has_letter('B')) target_ik_ab.beta = gcode->get_value('B');

	// If need IK, do it now.
	if (gcode->has_letter('Z')) {
		do_ik=true;
		target_fk_zw.Z = gcode->get_value('Z');
	}
	if (gcode->has_letter('W')){
		do_ik=true;
		target_fk_zw.W = gcode->get_value('W');
	}
	if (do_ik) IK(&target_fk_zw,&target_ik_ab);

	//Prepare actuator/driver to move to next point
	this->objStepper_alpha.setTargetAbs(target_ik_ab.alpha);
	this->objStepper_beta.setTargetAbs(target_ik_ab.beta);
	//None blocking, move backgroundly.
	this->objStepControl.moveAsync(this->objStepper_alpha, this->objStepper_beta);

	if (true){
		Serial.print("\n[Debug] GobotHouseHardware::RunG1() ");
		Serial.print(this->objStepper_alpha.getPosition());
		Serial.print(",");
		Serial.print(this->objStepper_beta.getPosition());
		Serial.print(" <-- from   alpha,beta   to --> ");
		Serial.print(target_ik_ab.alpha);
		Serial.print(">>");
		Serial.print(target_ik_ab.alpha  );
		Serial.print(" , ");
		Serial.println(target_ik_ab.beta);
	}







	// //None blocking, move backgroundly.
	// Serial.println("[Debug] BoxMoverHardware::RunG1()   \n");
	// if (gcode->has_letter('F')){
	// 	int speed = gcode->get_value('F');
	// 	this->objStepper_alpha.setMaxSpeed(speed);
	// }

    // FkPosition_ZW target_fk;
    // target_fk.Z = this->__current_fk_position.Z;
    // target_fk.W = this->__current_fk_position.W;

	// if (gcode->has_letter('A')) 
    //     target_fk.a = gcode->get_value('A');
	// if (gcode->has_letter('B')) 
    //     target_fk.z = gcode->get_value('Z');

    // Serial.print(this->__current_fk_position.z);
	// Serial.print(",");
	// Serial.println(this->__current_fk_position.a);
	// Serial.print(" <-- from   FK(Z, alpha)   to --> ");
	// Serial.print(target_fk.z);
	// Serial.print(",");
	// Serial.println(target_fk.a);
    
    // IkPosition_AB target_ik;
    // this->IK(&target_fk, &target_ik);

	// this->objStepper_alpha.setTargetAbs(target_ik.alpha);
	// this->objStepper_beta.setTargetAbs(target_ik.beta);
	// this->objStepControl.moveAsync(this->objStepper_alpha, this->objStepper_beta);

}
void BoxMoverHardware::_running_G1(){
    if (this->GetDistanceToTarget_IK() < MAX_ACCELERATION_ALPHPA + MAX_ACCELERATION_BETA){
      	this->State = IDLE;
		Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);
}
