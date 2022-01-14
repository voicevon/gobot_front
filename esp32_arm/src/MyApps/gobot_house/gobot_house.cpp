#include "gobot_house.h"

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

// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/


GobotHouse::GobotHouse(){
}

void GobotHouse::Setup(RobotAction* pAction, int segments){

	__house_action = pAction;
	__segments = segments;
	__map.setup();
}

FkPositionBase GobotHouse::GetCurrentPosition(){
	return this->__current_fk_position; 
}


void GobotHouse::HomeSingleAxis(char axis){
  Serial.print("[Debug] GobotHouse::HomeSingleAxis() is entering\n" );
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



void GobotHouse::_running_G28(){
	Serial.print("[Debug] GobotHouse::running_G28() is entering \n");
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print(" Homed postion =    " );
		this->objStepControl.stop();
		this->__homing_stepper->setPosition(0);
		Serial.println(this->__homing_stepper->getPosition());
		// if (this->_homing_axis == 'A')
		// 	this->objActuator_Alpha.SetCurrentPos(HOME_POSITION_ALPHA);
		// 	// this->
		// else if (this->_homing_axis == 'B')
		// 	this->objActuator_Beta.SetCurrentPos(HOME_POSITION_BETA);
		this->State = IDLE;
	}else{
		Serial.print("Still homing\n");
		// Endstop is not trigered
		this->__homing_stepper->setTargetRel(-5000);
		this->objStepControl.moveAsync(*this->__homing_stepper);
	}
}

// Head is a position name, The 5 bar arm will pick up stone from there.
void GobotHouse::MoveStone_FromRoomToHead(uint8_t room_id){
  __Move_fromNeck_toDoor(room_id,true);
  __Move_fromRoom_toDoor(room_id,false);
  __Enable_eefCoil(true);
  __Move_fromRoom_toDoor(room_id, true);
  __Move_fromNeck_toDoor(0, false);
  __Move_fromHead_toNeck(false);
  __Enable_eefCoil(false);
  __Move_fromHead_toNeck(true);
}

void GobotHouse::MoveStone_FromHeadToRoom(uint8_t room_id){
  __Move_fromNeck_toDoor(0, false);  // 0 is useless.
  __Move_fromHead_toNeck(false);
  __Enable_eefCoil(true);
  __Move_fromHead_toNeck(true);
  __Move_fromNeck_toDoor(room_id, true);
  __Move_fromRoom_toDoor(room_id, false);
  __Enable_eefCoil(false);
  __Move_fromRoom_toDoor(room_id, true);
}

void GobotHouse::__Enable_eefCoil(bool enable){

}


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
void GobotHouse::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosXY* fk = (FkPosXY*)(from_fk);
	IkPosAB* ik = (IkPosAB*)(to_ik);
	float rr1= fk->X * fk->X + fk->Y * fk->Y;

	float beta = acosf((LINK_A * LINK_A + LINK_B * LINK_B -  rr1 ) / (2* LINK_A * LINK_B));
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf(fk->X/r1);
	float alpha_link = acosf((LINK_A * LINK_A + rr1 - LINK_B * LINK_B)/( 2*LINK_A * r1));
	float alpha = alpha_eef + alpha_link;
	ik->alpha = alpha * STEPS_PER_RAD;
	ik->beta =  beta * STEPS_PER_RAD; 
}

void GobotHouse::FK(IkPositionBase* ik, FkPositionBase*  to_fk){

}


void GobotHouse::__Move_fromHead_toNeck(bool forwarding){
  float x1 = this->__map.head.x;
  float x2 = this->__map.neck.x;
  if(!forwarding){
	//from neck to head
	x1 = this->__map.neck.x;
	x2 = this->__map.head.x;
  }
  float distance = x2 - x1;
  float dx = distance / this->__segments ;
  for(int segment= 0; segment < this->__segments; segment++){
	float x = x1 + dx * segment;
	// MoveTo(x, 0);
	this->ActuatorMoveTo_FK(x,0);
  }
}

void GobotHouse::__Move_fromRoom_toDoor(uint8_t room_id, bool forwarding){
	//from room to door, now is at room.
  float x1 = __map.rooms[room_id].x;
  float y1 = __map.rooms[room_id].y;
  float x2 = __map.doors[room_id].x;
  float y2 = __map.doors[room_id].y;

  if (!forwarding){
	//from door to room, now is at door.
	x1 = __map.doors[room_id].x;
	y1 = __map.doors[room_id].y;
	x2 = __map.rooms[room_id].x;
	y2 = __map.rooms[room_id].y;
  }

  // float distance = __map.distance_room_to_door[room_id];
  float dx = (x2 - x1) / __segments;
  float dy = (y2 - y1) / __segments;
  for(int segment= 0; segment < __segments; segment++){
	float x = x1 + dx * segment;
	float y = y1 + dy * segment;
	// MoveTo(x,y);
	this->ActuatorMoveTo_FK(x,y);
  }
}

// This is almost a  rotation, because beta should be no changing.
void GobotHouse::__Move_fromNeck_toDoor(uint8_t room_id, bool forwarding){
  float x = __map.doors[room_id].x;
  float y = __map.doors[room_id].y;
  if (!forwarding){
	x = __map.neck.x;
	y = __map.neck.y;    
  }
  this->ActuatorMoveTo_FK(x,y);
}

void GobotHouse::init_gpio(){
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
void GobotHouse::Init_Linkage(){
  init_gpio();
  this->commuDevice = &this->objCommuUart; 

}

float GobotHouse::GetDistanceToTarget(){
	IkPosAB current_ik;
	current_ik.alpha = (float)this->objStepper_alpha.getPosition();
	current_ik.alpha = (float)this->objStepper_beta.getPosition();
	FK(&current_ik, &this->__current_fk_position);
	
	float dx = this->__current_fk_position.X - this->__target_fk_position.X;
	float dy = this->__current_fk_position.Y - this->__target_fk_position.Y;
	float distance = sqrt(dx * dx + dy * dy);
	return distance;
}

void GobotHouse::RunG1(Gcode* gcode) {
	//None blocking, move backgroundly.
	Serial.println("[Debug] GobotHouse::RunG1()   \n");
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		// this->objStepper.setMaxSpeed(speed);
	}
	float target_alpha = this->objStepper_alpha.getPosition();
	float target_beta = this->objStepper_beta.getPosition();
	Serial.print(target_alpha);
	if (gcode->has_letter('A')) target_alpha = gcode->get_value('A');
	if (gcode->has_letter('B')) target_beta = gcode->get_value('B');
	Serial.print(" <-- from   alpha-stepper   to --> ");
	Serial.println(target_alpha);

	this->objStepper_alpha.setTargetAbs(target_alpha);
	this->objStepper_beta.setTargetAbs(target_beta);
	this->objStepControl.move(this->objStepper_alpha, this->objStepper_beta);
}
void GobotHouse:: _running_G1(){
    if (this->GetDistanceToTarget() < 20){
      	this->State = IDLE;
    }
}

void GobotHouse::ActuatorMoveTo_FK(float x, float y){

}
void GobotHouse::ActuatorMoveTo_IK(int32_t a, int32_t b){
  
}
