#include "gobot_house_hw.h"

#define PIN_ALPHA_ENABLE 18
#define PIN_BETA_ENABLE 16
#define PIN_MICRIO_STEP_2 21
#define PIN_MICRIO_STEP_1 22
#define PIN_MICRIO_STEP_0 23



#define MOTOR_MAX_SPEED 289
#define HOME_POSITION_ALPHA 0
#define HOME_POSITION_BETA 1

#define LINK_A 75
#define LINK_B 75

#define STEPS_PER_RAD 326   //2048 / 2*Pi
#define ACCELERATION_HOMIMG 2000
#define MAX_SPEED_HOMING 2000

#define MAX_STEPS_PER_SECOND_ALPHA 500
#define MAX_STEPS_PER_SECOND_BETA 500
#define MAX_ACCELERATION_ALPHPA 200
#define MAX_ACCELERATION_BETA 200

// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/






/*

	House                Y+
						 ^
			 r0          |
		  r1     d0      |
		r2      d1       |
	  r3       d3  d2    |               
	  -----------------(0,0)------Neck----------Head    --> X+
	  r4      d4  d5     |
	   r5       d6       |
		 r6     d7       |
		  r7             |

*/

// }
void GobotHouseHardware::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosXY* fk = (FkPosXY*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);
	float rr1= fk->X * fk->X + fk->Y * fk->Y;

	float beta = acosf((LINK_A * LINK_A + LINK_B * LINK_B -  rr1 ) / (2* LINK_A * LINK_B));
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf(fk->X/r1);
	float alpha_link = acosf((LINK_A * LINK_A + rr1 - LINK_B * LINK_B)/( 2*LINK_A * r1));
	float alpha = alpha_eef + alpha_link;
	ik->alpha = alpha * STEPS_PER_RAD;
	ik->beta =  beta * STEPS_PER_RAD; 
}

void GobotHouseHardware::FK(IkPositionBase* ik, FkPositionBase*  to_fk){

}

GobotHouseHardware::GobotHouseHardware(){
}


bool GobotHouseHardware::GetCurrentPosition(FkPositionBase* position_fk){
	position_fk = & this->__current_fk_position;
	return true;
}


void GobotHouseHardware::init_gpio(){
	pinMode(PIN_ALPHA_ENABLE, OUTPUT);
	pinMode(PIN_BETA_ENABLE, OUTPUT);
	pinMode(PIN_MICRIO_STEP_0, OUTPUT);
	pinMode(PIN_MICRIO_STEP_1, OUTPUT);
	pinMode(PIN_MICRIO_STEP_2, OUTPUT);

	digitalWrite(PIN_ALPHA_ENABLE, LOW);
	digitalWrite(PIN_BETA_ENABLE, LOW);
	digitalWrite(PIN_MICRIO_STEP_0, LOW);
	digitalWrite(PIN_MICRIO_STEP_1, LOW);
	digitalWrite(PIN_MICRIO_STEP_2, LOW);

}
void GobotHouseHardware::Init_Linkage(){
	init_gpio();
	this->commuDevice = &this->objCommuUart; 
	this->objStepper_alpha.setAcceleration(MAX_ACCELERATION_ALPHPA);
	this->objStepper_alpha.setMaxSpeed(MAX_ACCELERATION_ALPHPA);
	this->objStepper_beta.setAcceleration(MAX_ACCELERATION_BETA);
	this->objStepper_beta.setMaxSpeed(MAX_STEPS_PER_SECOND_BETA);
	this->objStepper_beta.setInverseRotation(true);

}

float GobotHouseHardware::GetDistanceToTarget_FK(){
	// because in this arm solution,  FK is equal to IK. so never mind the logic error.
	// BUT: PLEASE DO NOT REFERENCE THESE CODES!!!
	// TODO: Rewrite this function.
	IkPosAB current_ik;
	current_ik.alpha = (float)this->objStepper_alpha.getPosition();
	current_ik.beta = (float)this->objStepper_beta.getPosition();
	FK(&current_ik, &this->__current_fk_position);
	
	float dx = this->__current_fk_position.X - this->__next_fk_position.X;
	float dy = this->__current_fk_position.Y - this->__next_fk_position.Y;
	float distance = sqrt(dx * dx + dy * dy);
	return distance;
}

float GobotHouseHardware::GetDistanceToTarget_IK(){
	int32_t da = this->objStepper_alpha.getDistanceToTarget();
	int32_t db = this->objStepper_beta.getDistanceToTarget();
	float distance = sqrt(da * da + db * db);
	return distance;
}

void GobotHouseHardware::RunG1(Gcode* gcode) {
	//None blocking, move backgroundly.
	Serial.println("[Debug] GobotHouseHardware::RunG1()   \n");
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha.setMaxSpeed(speed);
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
void GobotHouseHardware:: _running_G1(){
    if (this->GetDistanceToTarget_IK() < MAX_ACCELERATION_ALPHPA + MAX_ACCELERATION_BETA){
      	this->State = IDLE;
		Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);
}
void GobotHouseHardware::HomeSingleAxis(char axis){
  Serial.print("[Debug] GobotHouseHardware::HomeSingleAxis() is entering\n" );
  Serial.print(axis);
  this->_homing_axis = axis;
  if (axis=='A'){
	this->objStepper_alpha.setAcceleration(ACCELERATION_HOMIMG);
	this->objStepper_alpha.setMaxSpeed(MAX_SPEED_HOMING);
	this->__homing_stepper = &this->objStepper_alpha;
	this->__homing_helper = &this->objHomeHelper_alpha;
  }else if (axis=='B'){
	this->objStepper_beta.setAcceleration(ACCELERATION_HOMIMG);
	this->objStepper_beta.setMaxSpeed(MAX_SPEED_HOMING);
	this->__homing_stepper = &this->objStepper_beta;
	this->__homing_helper = &this->objHomeHelper_beta;
  }
}

void GobotHouseHardware::_running_G28(){
	// Serial.print("[Debug] GobotHouseHardware::running_G28() is entering \n");
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] Homed postion =  " );
		this->objStepControl.stop();
		this->__homing_stepper->setPosition(0);
		Serial.println(this->__homing_stepper->getPosition());
		this->State = IDLE;
		this->objStepper_alpha.setMaxSpeed(MAX_STEPS_PER_SECOND_ALPHA);
		this->objStepper_alpha.setAcceleration(MAX_ACCELERATION_ALPHPA);
		this->objStepper_beta.setMaxSpeed(MAX_STEPS_PER_SECOND_BETA);
		this->objStepper_beta.setAcceleration(MAX_ACCELERATION_BETA);

	}else{
		// Endstop is not trigered
		// Serial.print("[Debug] Still homing\n");
		// Serial.print("<");

		// We are going to move a long long distance with async mode(None blocking).
		// When endstop is trigered, must stop the moving. 
		this->__homing_stepper->setTargetRel(-5000);
		this->objStepControl.moveAsync(*this->__homing_stepper);
	}
}

