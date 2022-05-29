// #include "smoke_bot.h"

// #define ACCELERATION 250000
// #define MAX_SPEED 23000   //  is OK
// #define ACCELERATION_HOMIMG  250000
// #define MAX_SPEED_HOMING  5200

// SmokeBotHardware::SmokeBotHardware(){

// }

// void SmokeBotHardware::RunG1(Gcode* gcode) {
// 	//None blocking, move backgroundly.
// 	// Serial.println("---------  Running G1   ");
// 	float distance = gcode->get_value('X');
// 	if (gcode->has_letter('F')){
// 		int speed = gcode->get_value('F');
// 		this->objStepper.setMaxSpeed(speed);
// 		}

// 	this->objStepper.setTargetAbs(distance);
// 	this->objStepControl.move(this->objStepper);
// }

// void SmokeBotHardware:: _running_G1(){
// 	if (this->objStepper.getDistanceToTarget() < 20){
// 	this->State = CncState::IDLE;
// 	}
// }

// void SmokeBotHardware::HomeSingleAxis(char axis){
// 	Serial.println("\n================================  " );
// 	Serial.print(" Start homing    " );
// 	this->objStepper.setAcceleration(ACCELERATION_HOMIMG);
// 	this->objStepper.setMaxSpeed(MAX_SPEED_HOMING);
// }

// void SmokeBotHardware:: _running_G28(){
// 	if (this->objHomeHelper.IsTriged()){
// 		// End stop is trigered
// 	this->objStepControl.stop();
// 		this->objStepper.setPosition(0);
// 		this->objStepper.setAcceleration(ACCELERATION);
// 		this->objStepper.setMaxSpeed(MAX_SPEED);
// 		Serial.print(" Homed postion =    " );
// 		Serial.println(this->objStepper.getPosition());
// 		this->State = CncState::IDLE;
// 	}else{
// 		this->objStepper.setTargetRel(-5000);
// 		this->objStepControl.moveAsync(this->objStepper);
// 		delay(10); 
// 	}
	
// }

// void SmokeBotHardware::Init_Gpio(){
// 	Serial.println("Smoke bot is initializing GPIO ...");
// 	pinMode(PIN_ENABLE, OUTPUT);
// 	pinMode(MICRO_STEP_1, OUTPUT);
// 	pinMode(MICRO_STEP_2, OUTPUT);
// 	pinMode(MICRO_STEP_3, OUTPUT);

// 	digitalWrite(PIN_ENABLE, LOW);
// 	digitalWrite(MICRO_STEP_1, HIGH);
// 	digitalWrite(MICRO_STEP_2, HIGH);
// 	digitalWrite(MICRO_STEP_3, HIGH);

// 	this->objStepper.setStepPinPolarity(HIGH);
// }

// void SmokeBotHardware::InitRobot(){
// 	// this->commuDevice = &this->objCommuUart; 
// 	// this->objHomeHelper.LinkActuator(&this->objActuator);
// 	// this->objCommuUart.OutputMessage("Hoddy, I am commmunicator UART ....");

// }

// // void SmokeBotHardware::__EnableMotor(char actuator, bool enable_it){
// // 	// Only one stepper motor here.
// // 	digitalWrite(PIN_ENABLE, !enable_it);
// // }