#include<math.h>
#include "gobot_chessboard_hw.h"
#include<Arduino.h>

void GobotChessboardHardware::InitRobot(){
	// Serial.print("\n[Info] GobotChessboardHardware::Init() is entering.");
    pinMode(PIN_ALPHA_ENABLE, OUTPUT);
    pinMode(PIN_BETA_ENABLE, OUTPUT);
	this->__EnableMotor('A', false);
	this->__EnableMotor('B', false);

	CommuUart* objCommuUart = new CommuUart();
    this->commuDevice = objCommuUart;
	Serial.print("\n[Info] GobotChessboardHardware::Init() is done.");
} 

void GobotChessboardHardware::HomeSingleAxis(char axis){ 
	Serial.print("[Debug] GobotChessboardHardware::HomeSingleAxis() is entering\n" );
	Serial.print(axis);
	this->_homing_axis = axis;
	if (axis=='A'){
		this->__EnableMotor('A', true);
		this->objStepper_alpha.setAcceleration(this->__config.Homing_acceleration_alpha_beta);
		this->objStepper_alpha.setMaxSpeed(this->__config.Homing_speed_alpha_beta);
		this->__homing_stepper = &this->objStepper_alpha;
		this->__homing_helper = &this->objHomeHelper_alpha;
		this->__homing_stepper->setTargetRel(500000);    // angle to be greater.
	}else if (axis=='B'){
		this->__EnableMotor('B',true);
		this->objStepper_beta.setAcceleration(this->__config.Homing_acceleration_alpha_beta);
		this->objStepper_beta.setMaxSpeed(this->__config.Homing_speed_alpha_beta);
		this->__homing_stepper = &this->objStepper_beta;
		this->__homing_helper = &this->objHomeHelper_beta;
		this->__homing_stepper->setTargetRel(-500000);    //angle to be smaller.
	}else{
		Serial.print("\n[Error] GobotChessboardHardware::HomeSingleAxis() ");
	}
	this->objStepControl.moveAsync(*this->__homing_stepper);
	Serial.print("[Debug] GobotChessboardHardware::HomeSingleAxis() is Starting to run...\n" );
}


// When first axis is homed, should it park to somewhere? For the reason, LINK_B might not long enouth to park the second arm.
void GobotChessboardHardware::_running_G28(){
	// Serial.print("[Info] GobotHouseHardware::running_G28() is entering \n");

	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] GobotChessboardHardware::_running_G28() Home sensor is trigered.  " );
		Serial.print (this->_homing_axis);
		this->objStepControl.stop();

		// The homed postion is a Inverse kinematic position for alpha, beta.
		IkPosition_AB ik_position;
		if (this->_home_as_inverse_kinematic){
			Serial.print("\n   [Info] Trying to get home position from actuator position  ");
			ik_position.alpha =  this->__config.Homed_position_alpha_in_degree * DEG_TO_RAD;
			ik_position.beta =  this->__config.Homed_position_beta_in_degree * DEG_TO_RAD;
			this->FK(&ik_position, &this->__current_fk_position);
			// verify FK by IK()
			IkPosition_AB verifying_ik;
			Serial.print("\n\n  [Info] Please verify IK->FK->IK   ");
			this->IK(&this->__current_fk_position, &verifying_ik);
		}else{
			Serial.print("\n  [Error] Trying to get home position with EEF-FK position  ");
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis == 'A') 
			this->objStepper_alpha.setPosition(ik_position.alpha * this->__config.STEPS_PER_RAD);
		if (this->_homing_axis == 'B') 
			this->objStepper_beta.setPosition(ik_position.beta * this->__config.STEPS_PER_RAD);
		
		this->objStepper_alpha.setMaxSpeed(this->__config.MAX_STEPS_PER_SECOND_ALPHA_BETA);
		this->objStepper_alpha.setAcceleration(this->__config.MAX_ACCELERATION_ALPHA_BETA);
		this->objStepper_beta.setMaxSpeed(this->__config.MAX_STEPS_PER_SECOND_ALPHA_BETA);
		this->objStepper_beta.setAcceleration(this->__config.MAX_ACCELERATION_ALPHA_BETA);
		this->State = RobotState::IDLE;

	}else{
		// Endstop is not trigered
		// Serial.print("\n[Debug] GobotChessboardHardware::_running_G28()  Still homing\n");
		// We are going to move a long long distance with async mode(None blocking).
		// When endstop is trigered, must stop the moving. 

	}
}

// https://github.com/ddelago/5-Bar-Parallel-Robot-Kinematics-Simulation/blob/master/fiveBar_InvKinematics.py
void GobotChessboardHardware::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

	float rr1= (fk->X + this->__config.LINK_0) * (fk->X + this->__config.LINK_0) + fk->Y * fk->Y;
	// alpha , beta are in unit of RAD.
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf((fk->X + this->__config.LINK_0) / r1);
	bool output_debug = false;
	if (output_debug){
		Serial.println(this->__config.LINK_0);
		Serial.println(this->__config.LINK_A);
		Serial.println(this->__config.LINK_B);
		Serial.println(r1);
		Serial.println((this->__config.LINK_A * this->__config.LINK_A + rr1 - this->__config.LINK_B * this->__config.LINK_B) / ( this->__config.LINK_A * r1 * 2));
	}
	float alpha_link = acosf((this->__config.LINK_A * this->__config.LINK_A + rr1 - this->__config.LINK_B * this->__config.LINK_B) / ( this->__config.LINK_A * r1 * 2));
	ik->alpha = alpha_eef + alpha_link;

	float rr2 = (fk->X - this->__config.LINK_0)* (fk->X - this->__config.LINK_0) + fk->Y * fk->Y;
	float r2 = sqrtf(rr2);
	float beta_eef = acosf((fk->X - this->__config.LINK_0) / r2 );
	float beta_link = acosf((this->__config.LINK_A * this->__config.LINK_A + rr2 - this->__config.LINK_B *this->__config. LINK_B) / (this->__config.LINK_A * r2 * 2));
	ik->beta = beta_eef - beta_link;

  	Serial.print("\n[Debug] GobotHouseHardware::IK() from (X,Y)=(");
	Serial.print(fk->X);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")\n    Inverse kinematic angle degree of origin (ik->alpha, ik->beta)=( ");
	Serial.print(ik->alpha * RAD_TO_DEG);
	Serial.print(" , ");
	Serial.print(ik->beta * RAD_TO_DEG);
	Serial.print(")");
}

void GobotChessboardHardware::FK(IkPositionBase* from_ik, FkPositionBase* to_fk){
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	FkPosition_XY* fk = (FkPosition_XY*)(to_fk);

	float elbow_alpha_x = this->__config.LINK_A * cosf(ik->alpha) - this->__config.LINK_0;   // TODO:: whan alpha > 180 degree.
	float elbow_alpha_y = this->__config.LINK_A * sinf(ik->alpha);   // TODO:: When alpha > 90 degree
	float elbow_beta_x = this->__config.LINK_A * cosf(ik->beta) + this->__config.LINK_0;   //TODO: when alpha < 0 degree. 
	float elbow_beta_y = this->__config.LINK_A * sinf(ik->beta);     //TODO: When beta < -90 degree.
	
	bool output_debug = false;
	if (output_debug){
		Serial.print("\nelbow_alpja(x,y) = ( ");
		Serial.print(elbow_alpha_x);
		Serial.print(" , ");
		Serial.print(elbow_alpha_y);
		Serial.print(" )");

		Serial.print("\nelbow_beta(x,y) = ( ");
		Serial.print(elbow_beta_x);
		Serial.print(" , ");
		Serial.print(elbow_beta_y);
		Serial.print(" )");
	}


	float center_x = (elbow_alpha_x + elbow_beta_x) / 2;
	float center_y = (elbow_alpha_y + elbow_beta_y) / 2;
	if (output_debug){
		Serial.print("\nsegment_center(x,y) = ( ");
		Serial.print(center_x);
		Serial.print(" , ");
		Serial.print(center_y);
		Serial.print(" )");
	}
	// float slope = center_y /center_x;
	float delta_x = elbow_beta_x - elbow_alpha_x;
	float delta_y = elbow_beta_y - elbow_alpha_y;
	float origin_slope = delta_y / delta_x;
	float origin_angle = atanf(origin_slope);   // range in degree [-90..+90]
	float rotated_angle = origin_angle + PI / 2; // range in degree [0..180]
	if (output_debug){
		Serial.print("\ndelta(x,y) = ( ");
		Serial.print(delta_x);
		Serial.print(" , ");
		Serial.print(delta_y);
		Serial.print(" )");
		Serial.print("\norigin_slope , orign_angle, rotated_angle  = ( ");
		Serial.print(origin_slope);
		Serial.print(" , ");
		Serial.print(origin_angle * RAD_TO_DEG);	
		Serial.print(" , ");
		Serial.print(rotated_angle * RAD_TO_DEG);
		Serial.print(" )");
	}
	float elbows_distance_sqr = delta_x * delta_x + delta_y * delta_y;
	float lenth_from_center_to_eef = sqrtf(this->__config.LINK_B * this->__config.LINK_B - elbows_distance_sqr / 4);
	if (output_debug){
		Serial.print("\nelbow_distance and lenth_from_center_to_eef = ( ");
		Serial.print(sqrtf(elbows_distance_sqr));
		Serial.print(" , ");
		Serial.print(lenth_from_center_to_eef);
		Serial.print(" )");
	}
	fk->X = center_x + lenth_from_center_to_eef * cosf(rotated_angle);
	fk->Y = center_y + lenth_from_center_to_eef * sinf(rotated_angle);

	Serial.print("\n\n[Debug] GobotHouseHardware::FK()  in degree from (alpha,beta) =(");
	Serial.print(ik->alpha * RAD_TO_DEG);
	Serial.print(" , ");
	Serial.print(ik->beta * RAD_TO_DEG);
	Serial.print(") \n     Forward Kinematic result:  (X,Y)= (");
	Serial.print(fk->X);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")");
}

void GobotChessboardHardware::RunM123(uint8_t eef_channel, EefAction eef_action){
	switch (eef_action){
		case EefAction::Lower:
			eefServo->write(180);
			break;
		case EefAction::Higher:
			eefServo->write(0);
			break;
		case EefAction::Suck:
			digitalWrite(PIN_EEF_A, HIGH);
			digitalWrite(PIN_EEF_B, LOW);
			break;
		case EefAction::Release:
			digitalWrite(PIN_EEF_A,LOW);
			digitalWrite(PIN_EEF_B,HIGH);
			break;
		case EefAction::Sleep:
			digitalWrite(PIN_EEF_A,LOW);
			digitalWrite(PIN_EEF_B,LOW);
			break;
		default:
			break;

	}
}

void GobotChessboardHardware::RunG1(Gcode* gcode){
	Serial.print("\n[Debug] GobotChessboardHardware::RunG1()   ");
	Serial.print(gcode->get_command());
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha.setMaxSpeed(speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_XY target_fk_xy;
	IkPosition_AB target_ik_ab;
	target_fk_xy.X = this->__current_fk_position.X;
	target_fk_xy.Y = this->__current_fk_position.Y;
	target_ik_ab.alpha = this->objStepper_alpha.getPosition();
	target_ik_ab.beta = this->objStepper_beta.getPosition();
	bool do_ik=false;
	if (gcode->has_letter('A')){
		this->__EnableMotor('A', true); 
		target_ik_ab.alpha = gcode->get_value('A') * this->__config.STEPS_PER_RAD * DEG_TO_RAD;
	}
	if (gcode->has_letter('B')){
		this->__EnableMotor('B', true);
		target_ik_ab.beta = gcode->get_value('B') * this->__config.STEPS_PER_RAD * DEG_TO_RAD;
	}
	// If need IK, do it now.
	if (gcode->has_letter('X')) {
		do_ik=true;
		target_fk_xy.X = gcode->get_value('X');
	}
	if (gcode->has_letter('Y')){
		do_ik=true;
		target_fk_xy.Y = gcode->get_value('Y');
	}
	if (do_ik) IK(&target_fk_xy,&target_ik_ab);
	// Normally the unit in G1A,G1B is degree
	if(gcode->has_letter('R')) 
		// Bug now, the unit in G1A,G1B is RAD
		target_ik_ab.alpha = this->__config.MOTOR_STEPS_PER_SHAFT_ROUND * gcode->get_value('R');
	//Prepare actuator/driver to move to next point
	this->objStepper_alpha.setTargetAbs(target_ik_ab.alpha );
	this->objStepper_beta.setTargetAbs(target_ik_ab.beta);
	//None blocking, move backgroundly.
	this->objStepControl.moveAsync(this->objStepper_alpha, this->objStepper_beta);

	if (true){
		Serial.print("\n[Debug] GobotChessboardHardware::RunG1() ");
		Serial.print(this->objStepper_alpha.getPosition());
		Serial.print(",");
		Serial.print(this->objStepper_beta.getPosition());
		Serial.print(" <-- from   alpha,beta   to --> ");
		Serial.print(target_ik_ab.alpha);
		Serial.print(">>");
		Serial.print(target_ik_ab.alpha);
		Serial.print(" , ");
		Serial.println(target_ik_ab.beta);
	}  
}

void GobotChessboardHardware::_running_G1(){
    if (this->GetDistanceToTarget_IK() < (this->__config.MAX_STEPS_PER_SECOND_ALPHA_BETA)/32){
      	this->State = RobotState::IDLE;
		Serial.print("\n[Info] GobotChessboardHardware::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);  
}

float GobotChessboardHardware::GetDistanceToTarget_IK(){
	return this->objStepper_alpha.getDistanceToTarget() + this->objStepper_beta.getDistanceToTarget();
}

void GobotChessboardHardware::RunM84(){
	this->__EnableMotor('A', false);
	this->__EnableMotor('B', false);
}

void GobotChessboardHardware::__EnableMotor(char actuator, bool enable_it){
	if (actuator == 'A')
		digitalWrite(PIN_ALPHA_ENABLE, !enable_it);
	if (actuator == 'B')
		digitalWrite(PIN_BETA_ENABLE, !enable_it);
}
