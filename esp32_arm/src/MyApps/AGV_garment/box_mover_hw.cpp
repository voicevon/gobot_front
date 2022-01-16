#include "box_mover_hw.h"


#define STEPS_PER_RAD 326   //2048 / 2*Pi
#define ACCELERATION_HOMIMG 2000
#define MAX_SPEED_HOMING 2000

#define MAX_STEPS_PER_SECOND_ALPHA 500
#define MAX_STEPS_PER_SECOND_BETA 500
#define MAX_ACCELERATION_ALPHPA 20
#define MAX_ACCELERATION_BETA 20


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
	float target_alpha = this->objStepper_alpha.getPosition();
	float target_beta = this->objStepper_beta.getPosition();
	Serial.print(target_alpha);
	Serial.print(",");
	Serial.print(target_beta);
	if (gcode->has_letter('A')) target_alpha = gcode->get_value('A');
	if (gcode->has_letter('B')) target_beta = gcode->get_value('B');
	Serial.print(" <-- from   alpha,beta   to --> ");
	Serial.print(target_alpha);
	Serial.print(",");
	Serial.println(target_beta);

	this->objStepper_alpha.setTargetAbs(target_alpha);
	this->objStepper_beta.setTargetAbs(target_beta);
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
