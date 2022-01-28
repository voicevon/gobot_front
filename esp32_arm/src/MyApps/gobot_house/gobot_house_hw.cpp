#include "gobot_house_hw.h"

#define PIN_ALPHA_ENABLE 18
#define PIN_BETA_ENABLE 16
#define PIN_MICRIO_STEP_2 21
#define PIN_MICRIO_STEP_1 22
#define PIN_MICRIO_STEP_0 23


#define LINK_A 75
#define LINK_B 75

#define STEPS_PER_RAD_ALPHA 750   //2048 / 2*Pi
// #define MOTOR_MAX_SPEED_ALPHA 100
#define MAX_STEPS_PER_SECOND_ALPHA 200
#define MAX_ACCELERATION_ALPHPA 200
#define ACCELERATION_HOMIMG_ALPHA 200
#define MAX_SPEED_HOMING_ALPHA 200
#define HOME_POSITION_ALPHA 0

#define STEPS_PER_RAD_BETA 220   //2048 / 2*Pi
// #define MOTOR_MAX_SPEED_BETA 289
#define MAX_STEPS_PER_SECOND_BETA 500
#define MAX_ACCELERATION_BETA 200
#define ACCELERATION_HOMIMG_BETA 200
#define MAX_SPEED_HOMING_BETA 200
#define HOME_POSITION_BETA 1

// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/







// }
void GobotHouseHardware::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);
	float rr1= fk->X * fk->X + fk->Y * fk->Y;

	float beta = acosf((LINK_A * LINK_A + LINK_B * LINK_B -  rr1 ) / (2* LINK_A * LINK_B));
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf(fk->X/r1);
	float alpha_link = acosf((LINK_A * LINK_A + rr1 - LINK_B * LINK_B)/( 2*LINK_A * r1));
	float alpha = alpha_eef + alpha_link;
	ik->alpha = alpha ;
	ik->beta =  beta ; 
	Serial.print("IK result:");
	Serial.print(ik->alpha * 180 / 3.14);
	Serial.print(" , ");
	Serial.print(ik->beta * 180 / 3.14);
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
	IkPosition_AB current_ik;
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
	// TODO:  G1 X123 Y123, will use IK()
	Serial.println("[Debug] GobotHouseHardware::RunG1()   \n");
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha.setMaxSpeed(speed);
	}
	FkPosition_XY target_fk_xy;
	IkPosition_AB target_ik_ab;
	target_fk_xy.X = this->__current_fk_position.X;
	target_fk_xy.Y = this->__current_fk_position.Y;
	target_ik_ab.alpha = this->objStepper_alpha.getPosition();
	target_ik_ab.beta = this->objStepper_beta.getPosition();
	bool do_ik=false;
	if (gcode->has_letter('A')) target_ik_ab.alpha = gcode->get_value('A');
	if (gcode->has_letter('B')) target_ik_ab.beta = gcode->get_value('B');
	if (gcode->has_letter('X')) {
		do_ik=true;
		target_fk_xy.X = gcode->get_value('X');
	}
	if (gcode->has_letter('Y')){
		do_ik=true;
		target_fk_xy.Y = gcode->get_value('Y');
	}
	if (do_ik){

		IK(&target_fk_xy,&target_ik_ab);
	}
	if (true){
		Serial.print(this->objStepper_alpha.getPosition());
		Serial.print(",");
		Serial.print(this->objStepper_beta.getPosition());
		Serial.print(" <-- from   alpha,beta   to --> ");
		Serial.print(target_ik_ab.alpha);
		Serial.print(",");
		Serial.println(target_ik_ab.beta);
	}

	this->objStepper_alpha.setTargetAbs(target_ik_ab.alpha  * STEPS_PER_RAD_ALPHA);
	this->objStepper_beta.setTargetAbs(target_ik_ab.beta * STEPS_PER_RAD_BETA);
	this->objStepControl.moveAsync(this->objStepper_alpha, this->objStepper_beta);
}
void GobotHouseHardware:: _running_G1(){
    if (this->GetDistanceToTarget_IK() < (MAX_ACCELERATION_ALPHPA + MAX_ACCELERATION_BETA)/64){
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
		this->objStepper_alpha.setAcceleration(ACCELERATION_HOMIMG_ALPHA);
		this->objStepper_alpha.setMaxSpeed(MAX_SPEED_HOMING_ALPHA);
		this->__homing_stepper = &this->objStepper_alpha;
		this->__homing_helper = &this->objHomeHelper_alpha;
	}else if (axis=='B'){
		this->objStepper_beta.setAcceleration(ACCELERATION_HOMIMG_BETA);
		this->objStepper_beta.setMaxSpeed(MAX_SPEED_HOMING_BETA);
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
		this->__homing_stepper->setTargetRel(-50000);
		this->objStepControl.moveAsync(*this->__homing_stepper);

	}
}

