#include<math.h>
#include "gobot_chessboard_hw.h"
#include<Arduino.h>


GobotChessboardHardware::GobotChessboardHardware(){

}

void GobotChessboardHardware::Init(){
    pinMode(PIN_ALPHA_ENABLE, OUTPUT);
    pinMode(PIN_BETA_ENABLE, OUTPUT);
    digitalWrite(PIN_ALPHA_ENABLE, LOW);
    digitalWrite(PIN_BETA_ENABLE, LOW);

    this->commuDevice = &this->objCommuUart;
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
	}
}

void GobotChessboardHardware::_running_G28(){
	// Serial.print("[Debug] GobotHouseHardware::running_G28() is entering \n");
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] GobotChessboardHardware::_running_G28() Home sensor is trigger.  " );
		Serial.print (this->_homing_axis);
		this->objStepControl.stop();

		//Set current position to HomePosition
		IkPosition_AB ik_position;
		if (this->_home_as_inverse_kinematic){
			Serial.print("\n   [Info] Trying to get home position from actuator position  ");
			ik_position.alpha = PI * this->__config.Homed_position_alpha_in_degree * this->__config.steps_per_rad /180;
			ik_position.beta = PI * this->__config.Homed_position_beta_in_degree * this->__config.steps_per_rad /180;
			this->FK(&ik_position, &this->__current_fk_position);
			// verify FK by IK()
			IkPosition_AB verifying_ik;
			Serial.print("\n\n  [Info] Please verify the below output ======================  ");
			this->IK(&this->__current_fk_position, &verifying_ik);
		}
		else{
			Serial.print("\n  [Info] Trying to get home position with EEF position  ");
			this->__current_fk_position.X = this->__config.Homed_position_x;
			this->__current_fk_position.Y = this->__config.Homed_position_y;
			this->IK(&this->__current_fk_position, &ik_position);
			// verify IK by FK()
			FkPosition_XY verifying_fk;
			Serial.print("\n   [Info] Please verify the below output ======================  ");
			this->FK(&ik_position, &verifying_fk);
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
		// Serial.print("[Debug] Still homing\n");
		// Serial.print("<");
		// We are going to move a long long distance with async mode(None blocking).
		// When endstop is trigered, must stop the moving. 
		this->__homing_stepper->setTargetRel(50000);
		this->objStepControl.moveAsync(*this->__homing_stepper);
	}
}

// void GobotChessboardHardware::__HomeSpin(Stepper* homing_stepper, uint8_t home_pin ){
// }

/*
https://github.com/ddelago/5-Bar-Parallel-Robot-Kinematics-Simulation/blob/master/fiveBar_InvKinematics.py
*/
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

void GobotChessboardHardware::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
  FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
  IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

  float rr1= (fk->X + this->__config.LINK_0) * (fk->X + this->__config.LINK_0) + fk->Y * fk->Y;
  
  float r1 = sqrtf(rr1);
  float alpha_eef = acosf((fk->X + this->__config.LINK_0) / r1);
  float alpha_link = acosf((this->__config.LINK_A * this->__config.LINK_A + rr1 - this->__config.LINK_B * this->__config.LINK_B) / ( 2*this->__config.LINK_A * r1));
  float alpha = alpha_eef + alpha_link;
  ik->alpha  = alpha * this->__config.steps_per_rad;

  float rr2 = (fk->X - this->__config.LINK_0)* (fk->X - this->__config.LINK_0) + fk->Y * fk->Y;
  float r2 = sqrtf(rr2);
  float beta_eef = acosf((fk->X - this->__config.LINK_0) / r2 );
  float beta_link = acosf((this->__config.LINK_A * this->__config.LINK_A + rr2 - this->__config.LINK_B *this->__config. LINK_B) / (2 * this->__config.LINK_A * r2));
  float beta = beta_eef - beta_link;
  ik->beta =  beta * this->__config.steps_per_rad; 
}

void GobotChessboardHardware::FK(IkPositionBase* from_ik, FkPositionBase* to_fk){

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