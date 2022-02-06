#include<math.h>
#include "gobot_chessboard_hw.h"
#include<Arduino.h>



void GobotChessboardHardware::Init(){
	Serial.print("\n[Info] GobotChessboardHardware::Init() is entering.");
    pinMode(PIN_ALPHA_ENABLE, OUTPUT);
    pinMode(PIN_BETA_ENABLE, OUTPUT);
    digitalWrite(PIN_ALPHA_ENABLE, LOW);
    digitalWrite(PIN_BETA_ENABLE, LOW);

	CommuUart* objCommuUart = new CommuUart();
    this->commuDevice = objCommuUart;
	Serial.print("\n[Info] GobotChessboardHardware::Init() is done.");
} 
void GobotChessboardHardware::HomeSingleAxis(char axis){ 
	Serial.print("[Debug] GobotChessboardHardware::HomeSingleAxis() is entering\n" );
	Serial.print(axis);
	this->_homing_axis = axis;
	if (axis=='A'){
		this->objStepper_alpha.setAcceleration(this->__config.Homing_acceleration_alpha);
		this->objStepper_alpha.setMaxSpeed(this->__config.Homing_speed_alpha);
		this->__homing_stepper = &this->objStepper_alpha;
		this->__homing_helper = &this->objHomeHelper_alpha;
	}else if (axis=='B'){
		this->objStepper_beta.setAcceleration(this->__config.Homing_acceleration_beta);
		this->objStepper_beta.setMaxSpeed(this->__config.Homing_speed_beta);
		this->__homing_stepper = &this->objStepper_beta;
		this->__homing_helper = &this->objHomeHelper_beta;
	}else{
		Serial.print("\n[Error] GobotChessboardHardware::HomeSingleAxis() ");
	}
	Serial.print("[Debug] GobotChessboardHardware::HomeSingleAxis() is done\n" );
}

void GobotChessboardHardware::_running_G28(){
	// Serial.print("[Info] GobotHouseHardware::running_G28() is entering \n");
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] GobotChessboardHardware::_running_G28() Home sensor is trigered.  " );
		Serial.print (this->_homing_axis);
		this->objStepControl.stop();

		//Set current position to HomePosition
		IkPosition_AB ik_position;
		if (this->_home_as_inverse_kinematic){
			Serial.print("\n   [Info] Trying to get home position from actuator position  ");
			ik_position.alpha = PI * this->__config.Homed_position_alpha_in_degree /180;
			ik_position.beta = PI * this->__config.Homed_position_beta_in_degree /180;
			this->FK(&ik_position, &this->__current_fk_position);
			// verify FK by IK()
			IkPosition_AB verifying_ik;
			Serial.print("\n\n  [Info] Please verify IK->FK->IK   ");
			this->IK(&this->__current_fk_position, &verifying_ik);
		}else{
			Serial.print("\n  [Error] Trying to get home position with EEF-FK position  ");
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis == 'A') this->objStepper_alpha.setPosition(ik_position.alpha);
		if (this->_homing_axis == 'B') this->objStepper_beta.setPosition(ik_position.beta);
		
		this->objStepper_alpha.setMaxSpeed(MAX_STEPS_PER_SECOND_ALPHA);
		this->objStepper_alpha.setAcceleration(MAX_ACCELERATION_ALPHPA);
		this->objStepper_beta.setMaxSpeed(MAX_STEPS_PER_SECOND_BETA);
		this->objStepper_beta.setAcceleration(MAX_ACCELERATION_BETA);
		this->State = IDLE;

	}else{
		// Endstop is not trigered
		// Serial.print("\n[Debug] GobotChessboardHardware::_running_G28()  Still homing\n");
		// We are going to move a long long distance with async mode(None blocking).
		// When endstop is trigered, must stop the moving. 
		this->__homing_stepper->setTargetRel(50000);
		this->objStepControl.moveAsync(*this->__homing_stepper);
	}
}



// IkPositionBase GobotChessboardHardware::IK(FkPosXY* fk){
//   IkPosXY ret;   //is risk here?
//   float rr1= (fk.x + LINK_0) * (fk.x + LINK_0) + fk.y * fk.y;
  
//   // float beta = acosf((LINK_A * LINK_A + LINK_B * LINK_B -  rr1 ) / (2* LINK_A * LINK_B));
//   float r1 = sqrtf(rr1);
//   float alpha_eef = acosf((fk.x + LINK_0) / r1);
//   float alpha_link = acosf((LINK_A * LINK_A + rr1 - LINK_B * LINK_B) / ( 2*LINK_A * r1));
//   float alpha = alpha_eef + alpha_link;
//   ret.alpha = alpha * STEPS_PER_RAD;

//   float rr2 = (fk.x - LINK_0)* (fk.x - LINK_0) + fk.y * fk.y;
//   float r2 = sqrtf(rr2);
//   float beta_eef = acosf((fk.x - LINK_0) / r2 );
//   float beta_link = acosf((LINK_A * LINK_A + rr2 - LINK_B * LINK_B) / (2 * LINK_A * r2));
//   float beta = beta_eef - beta_link;
//   ret.beta =  beta * STEPS_PER_RAD; 
//   return ret;
// }



// https://github.com/ddelago/5-Bar-Parallel-Robot-Kinematics-Simulation/blob/master/fiveBar_InvKinematics.py

void GobotChessboardHardware::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

	float rr1= (fk->X + this->__config.LINK_0) * (fk->X + this->__config.LINK_0) + fk->Y * fk->Y;
	// alpha , beta are in unit of RAD.
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf((fk->X + this->__config.LINK_0) / r1);
	Serial.println(this->__config.LINK_0);
	Serial.println(this->__config.LINK_A);
	Serial.println(this->__config.LINK_B);
	Serial.println(r1);
	Serial.println((this->__config.LINK_A * this->__config.LINK_A + rr1 - this->__config.LINK_B * this->__config.LINK_B) / ( this->__config.LINK_A * r1 * 2));
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
	Serial.print(ik->alpha * 180 / PI);
	Serial.print(" , ");
	Serial.print(ik->beta * 180 / PI);
	Serial.print(")");
}

void GobotChessboardHardware::FK(IkPositionBase* from_ik, FkPositionBase* to_fk){
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	FkPosition_XY* fk = (FkPosition_XY*)(to_fk);

	float elbow_alpha_x = this->__config.LINK_A * cosf(ik->alpha) - this->__config.LINK_0;   // TODO:: whan alpha > 180 degree.
	float elbow_alpha_y = this->__config.LINK_A * sinf(ik->alpha);   // TODO:: When alpha > 90 degree
	float elbow_beta_x = this->__config.LINK_A * cosf(ik->beta) + this->__config.LINK_0;   //TODO: when alpha < 0 degree. 
	float elbow_beta_y = this->__config.LINK_A * sinf(ik->beta);     //TODO: When beta < -90 degree.
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


	float center_x = (elbow_alpha_x + elbow_beta_x) / 2;
	float center_y = (elbow_alpha_y + elbow_beta_y) / 2;
	Serial.print("\n segment_center(x,y) = ( ");
	Serial.print(center_x);
	Serial.print(" , ");
	Serial.print(center_y);
	Serial.print(" )");

	// float slope = center_y /center_x;
	float delta_x = elbow_beta_x - elbow_alpha_x;
	float delta_y = elbow_beta_y - elbow_alpha_y;
	float slope = delta_y / delta_x;
	float angle = atanf(- 1.0f / slope);   // range in degree [-90..+90]
	if (angle <0) angle += PI / 2;       // range in degree [0..180]
	Serial.print("\ndelta(x,y) = ( ");
	Serial.print(delta_x);
	Serial.print(" , ");
	Serial.print(delta_y);
	Serial.print(" )");
	Serial.print("\nslope and angle = ( ");
	Serial.print(slope);
	Serial.print(" , ");
	Serial.print(PI * angle /180);
	Serial.print(" )");


	float elbows_distance_sqr = delta_x * delta_x + delta_y * delta_y;
	float lenth_from_center_to_eef = sqrtf(this->__config.LINK_B * this->__config.LINK_B - elbows_distance_sqr / 4);
	Serial.print("\nelbow_distance and lenth_from_center_to_eef = ( ");
	Serial.print(sqrtf(elbows_distance_sqr));
	Serial.print(" , ");
	Serial.print(lenth_from_center_to_eef);
	Serial.print(" )");

	fk->X = center_x + lenth_from_center_to_eef * cosf(angle);
	fk->Y = center_y + lenth_from_center_to_eef * sinf(angle);

	Serial.print("\n\n[Debug] GobotHouseHardware::FK()  in degree from (alpha,beta) =(");
	Serial.print(ik->alpha * 180 / PI);
	Serial.print(" , ");
	Serial.print(ik->beta * 180 / PI);
	Serial.print(") \n     Forward Kinematic result:  (X,Y)= (");
	Serial.print(fk->X);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")");
}

void GobotChessboardHardware::SetEffector(EEF action){
  switch (action){
    case Lower:
      eefServo->write(180);
      break;
    case Higher:
      eefServo->write(0);
      break;
    case Suck:
      digitalWrite(PIN_EEF_A, HIGH);
      digitalWrite(PIN_EEF_B, LOW);
      break;
    case Release:
      digitalWrite(PIN_EEF_A,LOW);
      digitalWrite(PIN_EEF_B,HIGH);
      break;
    case Sleep:
      digitalWrite(PIN_EEF_A,LOW);
      digitalWrite(PIN_EEF_B,LOW);
      break;
    default:
      break;

  }
}





void GobotChessboardHardware::RunG1(Gcode* gcode){
  
}

void GobotChessboardHardware::_running_G1(){
  
}