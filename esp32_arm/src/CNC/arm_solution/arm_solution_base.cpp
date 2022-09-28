#include "arm_solution_base.h"
#include "MyLibs/MyFunctions.hpp"
#include "HardwareSerial.h"
#include "MyLibs/message_queue.h"


void ArmSolutionBase::SayHello(){
	Serial.println("[Debug] ArmSolutionBase::SayHello()");
}

void ArmSolutionBase::SpinOnce(){
	// Logger::Debug("ArmSolutionBase::SpinOnce()");
	// Logger::Print("this->State", this->State);
	switch (this->State){
	case CncState::IDLE:   //0
		break;
	case CncState::RUNNING_G4:  // 2
		this->__running_G4();
		break;
	case CncState::RUNNING_G1:  // 1
		this->_running_G1();
		break;
	case CncState::RUNNING_G28:  // 4
		// Logger::Debug("CncState::RUNNING_G28");
		this->_running_G28();
		break;
	default:
		Logger::Warn("ArmSolutionBase::SpinOnce() Unknown current state: ");
		break;
	}

	// Serial.println("[Debug]( ArmSolutionBase::SpinOnce() is finished.)");
	// this->SpinOnce_BaseExit();
}

void ArmSolutionBase::ForceStopMover(){
	this->_mover_base->AllActuatorsStop();
}




//Can deal with:  home via single actuator.
//Can NOT deal with:  CoreXY, It's combined moving.
void ArmSolutionBase::RunG28(EnumAxis axis){ 
	bool debug = true;
	if (debug){
		Logger::Debug("ArmSolutionBase::RunG28() is entering" );
		Logger::Print("axis", axis);
		Logger::Print("IsCombinedFK", this->_config_base.IsCombinedFk);
	}

	if (this->_config_base.IsCombinedFk){
		this->_RunG28_CombinedFk(axis);

	}else{
		this->__HomeSingleAxis(axis);
	}
}


void ArmSolutionBase::__HomeSingleAxis(EnumAxis axis){
	Logger::Debug("ArmSolutionBase::__HomeSingleAxis()");
	this->_homing_axis = axis;
	HomingConfig* homing = this->_cnc_homer.GetAxisHomer(axis)->GetHomingConfig();
	this->_cnc_board->EnableMotor(axis, true);
	
	this->_config_base.PrintOut("ArmSolutionBase::__HomeSingleAxis()  _config_base");
	this->_mover_base->PrintOut("ArmSolutionBase::__HomeSingleAxis()  _mover_base" );
	this->_mover_base->SetActuatorSpeed(axis, homing->Speed);
	this->_mover_base->SetActuatorAcceleration(axis, homing->Accelleration);
	// this->_mover_base->SingleActuatorMoveTo(axis, false, homing->DistanceToGo);
	MoveBlock line;
	line.axis = axis;
	line.IsAbsTargetPosition = false;
	line.TargetPosition = homing->DistanceToGo;
	line.Speed = homing->Speed;
	this->_mover_base->SingleActuatorMoveTo(&line);   //TOdo:  Put this line to line_queue

	Logger::Debug("ArmSolutionBase::RunG28() is Starting to run..." );
}

void ArmSolutionBase::_running_G28(){
	bool debug = false;
	if(debug){
		Logger::Debug("ArmSolutionBase::_running_G28() is entering...");
		Logger::Print("_homing_axis", this->_homing_axis);
		auto homer = this->_cnc_homer.GetAxisHomer(this->_homing_axis);
		Logger::Print("Got axis_homer",true);
		auto index = homer->GetTrigeredIndex();
		Logger::Print("Got triggered index", index);
	}

	int fired_trigger_index =  this->_cnc_homer.GetAxisHomer(this->_homing_axis)->GetTrigeredIndex();
	if (fired_trigger_index >=0 ){
		// End stop is trigered
		Logger::Info("ArmSolutionBase::_running_G28() ----> Home sensor is triggered." );
		Logger::Print("_homing_axis_name", this->_homing_axis);
		Logger::Print(" fired_trigger_index", fired_trigger_index);

		this->_mover_base->AllActuatorsStop();
		this->_SetCurrentPositionAsHome(this->_homing_axis);
		this->State = CncState::IDLE;
	}else{
		// Endstop is not trigered
		// Serial.print(".");
		delay(10);
	}
}

void ArmSolutionBase::_running_G1(){
	if (this->_mover_base->HasArrivedTargetPosition()){
	// float distance_to_target = this->GetDistanceToTarget_IK();
    // if (distance_to_target < DEG_TO_RAD * 1.0f){  // TODO:  == 0  or decide by subclass.

      	this->State = CncState::IDLE;
		Logger::Info("ArmSolutionBase::_running_G1() is finished. ");
		// Logger::Print("distance_to_target", RAD_TO_DEG * distance_to_target);
    }
}

void ArmSolutionBase::RunG4(Gcode* gcode){
	__g4_start_timestamp = micros();
	__g4_time_second = gcode->get_value('S');
}

void ArmSolutionBase::__running_G4(){
	long delayed = (micros() - __g4_start_timestamp) / 1000 /1000;
	if (delayed >= __g4_time_second ){
		this->State = CncState::IDLE;
		return;
	}
}

// void ArmSolutionBase::RunM123(uint8_t eef_channel, uint8_t eef_action){
// 	Logger::Debug("ArmSolutionBase::RunM123()");
// 	Logger::Print("eef_action", eef_action);
// 	// uint8_t action_code = 1;
// 	this->__eef->PrintOut();
// 	this->__eef->Run(eef_action);
// }

void ArmSolutionBase::RunM84(){
	//TODO: CNC_AXIS_COUNT_IK,   vs CNC_AXIS_COUNT_FK
	for (int axis=0; axis<CNC_AXIS_COUNT; axis++){
		this->_cnc_board->EnableMotor(EnumAxis(axis), false);
	}
}
void ArmSolutionBase::RunGcode(Gcode* gcode){
	std::string result;
	// if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND)){
	//   Serial.print("RunGcode()   OK or Unknown");
	//   return;
	// }
	bool debug = false;
	Logger::Debug("ArmSolutionBase::RunGcode()");
	// Logger::Print("gcode_command", gcode->get_command());
	Serial.println(gcode->get_command());

	if(gcode->has_g){
		char home_axis = '+';
		switch (gcode->g){
		case 28:
			// G28: Home
			this->State = CncState::RUNNING_G28;
			if (gcode->has_letter('X')) home_axis='X';
			if (gcode->has_letter('Y')) home_axis='Y';
			if (gcode->has_letter('Z')) home_axis='Z';
			if (gcode->has_letter('A')) home_axis='A';
			if (gcode->has_letter('B')) home_axis='B';
			if (gcode->has_letter('C')) home_axis='C';
			if (gcode->has_letter('W')) home_axis='W';

			// Is there any machine that supports both IK, and FK homing?
			// this->_home_via_inverse_kinematic = false;
			if (home_axis == '+'){
				Serial.print("\n\n\n\n[Error] ArmSolutionBase::RunGcode()  :");
				Serial.print(home_axis);

			}
			//TODO:  convert char to enum
			this->RunG28(this->ConvertToEnum(home_axis));
			// this->commuDevice->OutputMessage(COMMU_OK);  For calble-bot-corner, it should be 'Unknown Command'
			break;

		case 1:
			// G1 Move
			//TODO:  1. put position to movement queue. called "plan" in smoothieware? 
			//       2. send out OK.
			//       3. Set status to busy.
			//       4. Start Moving.
			this->State = CncState::RUNNING_G1;
			this->RunG1(gcode);
			// this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 4:
			// G4 Dwell, Pause for a period of time.
			this->State = CncState::RUNNING_G4;
			this->RunG4(gcode);
			break;
		case 6:
			this->RunG6(gcode);
			// this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 90:
			// Absolute position
			this->is_absolute_position = true;
			// this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 91:
			// Relative position
			this->is_absolute_position = false;
			// this->commuDevice->OutputMessage(COMMU_OK);
			break;
		// case 92:
			// Set Position     G92 X10 E90
			// break;
		default:
			break;
		}
	}else if(gcode->has_m){
		uint8_t p_value = 33;   //TODO: Make sure this is no harmful!
		uint8_t s_value = 0;
		float f_value = 0.0f;
		EefAction action;
		switch (gcode->m){
		case 42:
			p_value =  gcode->get_value('P');
			s_value = gcode->get_value('S');
			this->Run_M42_OutputGpio(p_value, s_value);
		case 84:
			this->RunM84();
		case 114:
			// Get Current Position
			break;

		// case 119:
		// 	// Get Endstop Status
		// 	result = GetHomeTrigerStateString();
		// 	// this->commuDevice->OutputMessage(COMMU_OK);
		// 	Serial.print(result.c_str());
		// 	// this->commuDevice->WriteNotification(result);
		// 	break;

		// case 123:
		// 	//M123 P=channel_index, S=Set EEF action			
		// 	while (this->State != CncState::IDLE){
		// 		this->SpinOnce();
		// 	}
		// 	p_value =  gcode->get_value('P');
		// 	s_value = gcode->get_value('S');
		// 	debug = true;
		// 	if (debug){
		// 		Logger::Debug("ArmSolutionBase::RunGcode() For EEF_ACTION  M123 ");
		// 		Logger::Print("P", p_value);
		// 		Logger::Print("S", s_value);
		// 	}
		// 	action = (EefAction)s_value;
		// 	this->RunM123(p_value, s_value);
		// 	break;

		// case 130:
		// 	Logger::Debug("ArmSolutionBase::RunGcode()   M130");
		// 	Logger::Print("gcode", gcode->get_command());
		// 	p_value =  gcode->get_value('N');
		// 	Logger::Print("Index", p_value);
			
		// 	f_value = gcode->get_value('P');
		// 	this->__pid_controllers->GetController(p_value)->P = f_value;
		// 	Logger::Print("P", f_value);

		// 	f_value = gcode->get_value('I');
		// 	this->__pid_controllers->GetController(p_value)->I = f_value;
		// 	Logger::Print("I", f_value);

		// 	f_value = gcode->get_value('D');
		// 	this->__pid_controllers->GetController(p_value)->D = f_value;
		// 	Logger::Print("D", f_value);

		// 	break;

		case 141:
			break;

		// case 280:
		// 	// Set servo position  
		// 	//	 	Pnnn Servo index
		// 	// 		Snnn Angle or microseconds
		// 	// Wait for all gcode, mcode is finished
		// 	// Serial.println("M280 Started");
		// 	while (this->State != CncState::IDLE){
		// 		this->SpinOnce();
		// 	}
		// 	if (gcode->has_letter('P')) p_value = gcode->get_value('P');
		// 	if (gcode->has_letter('S')) s_value = gcode->get_value('S');
		// 	ledcWrite(p_value, s_value);   // from ledcWrite(ledChannel, dutyCycle);
		// 	// this->commuDevice->OutputMessage(COMMU_OK);
		// 	// this->commuDevice->WriteNotification("IDLE");
		// 	break;
		case 996:
			// Do nothing. this should be the last gcode of a movement in transaction.
			// after MCU reset, This should be the first gcode it received, even the message queue is nothing.
			break;
		default:
			break;
		}
	}else{
	}
	Logger::Print("ArmSolutionBase::RunGcode() is finished", 0);
}


void ArmSolutionBase::Run_M42_OutputGpio(uint8_t pin_number, uint8_t pin_value){
	digitalWrite(pin_number, pin_value);
}
