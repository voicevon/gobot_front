// #include "robot_hardware_base.h"
// #include "cnc_base.h"
#include "solution_base.h"
#include "MyLibs/MyFunctions.hpp"
#include "HardwareSerial.h"
#include "MyLibs/message_queue.h"
// void CncSolutionBase::FeedMessage(char* message, int length){
// 	Gcode gcode = Gcode(message);
// 	this->RunGcode(&gcode);
// }
void CncSolutionBase::SayHello(){
	Serial.println("[Debug] CncSolutionBase::SayHello()");
}

void CncSolutionBase::SpinOnce(){
	// Serial.print("[Debug] CncSolutionBase::SpinOnce() is entering.  Current state= ");
	// Serial.println(this->State);
	switch (this->State){
	case CncState::IDLE:
		break;
	case CncState::RUNNING_G4:
		this->__running_G4();
		break;
	case CncState::RUNNING_G1:
		this->_running_G1();
		break;
	case CncState::RUNNING_G28:
		this->_running_G28();
		break;
	default:
		Serial.print("[Warning] CncSolutionBase::SpinOnce() Unknown current state: ");
		// Serial.print(this->State);
		break;
	}

	// Serial.println("[Debug]( CncSolutionBase::SpinOnce() is finished.)");
	this->SpinOnce_BaseExit();
}

// Check gcode queue, if there is gcode to be run.
void CncSolutionBase::SpinOnce_BaseExit(){
	if (this->State != CncState::IDLE)
		return;
	if (this->_gcode_queue->BufferIsEmpty())
		return;

	MessageQueue::SingleMessage* message = this->_gcode_queue->FetchTailMessage();
	if (message == NULL){
		Serial.println("\n\n\n [Error] CncSolutionBase::SpinOnce_BaseExit() tail_message is null. \n\n ");
		return;
	}

	bool debug = false;
	if (debug){
		Logger::Debug("[Info] CncSolutionBase::SpinOnce_BaseExit()  Going to run next gcode   ===> ");
		Serial.print(message->payload);
		Serial.println(" ");
	}
	
	// type convert   from char* to std::string
	char* p = &message->payload[0];
	std::string str = std::string(p);
	// feed std::string to Gcode constructor.
	Gcode gcode = Gcode(str);
	this->RunGcode(&gcode);
}


void CncSolutionBase::__HomeSingleAxis(EnumAxis axis){
	Logger::Debug("CncSolutionBase::__HomeSingleAxis()");
	Serial.print(axis);

	if ( axis==AXIS_ALPHA || axis == AXIS_BETA){
		this->_homing_axis_name = axis;
		this->__current_homer = this->_board->GetHomer(axis);
		Serial.print("1111111111111111111");
		CncSolutionConfigBase* config = this->_board->GetCncConfig();
		Serial.print("2222222222222");
		CncMoverBase* mover = this->_board->cnc_mover;
		config->PrintOut("Config in CncFiveBars::RunG28()");
		mover->SetActuatorSpeed(axis, config->HomingSpeed(axis));
		this->_board->EnableMotor(axis, true);
		float long_distance_to_move = 999.0f * config->HomingDir_IsToMax(axis);

		mover->SingleActuatorMoveTo(axis, false, long_distance_to_move);
		mover->PrintOut("Mover in CncSolutionBase::RunG28()");
	}else{
		Logger::Error("CncSolutionBase::RunG28() ");
	}
	Serial.println("[Debug] CncSolutionBase::RunG28() is Starting to run..." );
}


//Can deal with:  home via single actuator.
//Can NOT deal with:  CoreXY, It's combined moving.
void CncSolutionBase::RunG28(EnumAxis axis){ 
	Serial.print("[Debug] CncFiveBars::RunG28() is entering  axis= " );
	Serial.println(axis);
	Serial.print(" IsCombinedFK= ");
	Serial.println(this->_config->IsCombinedFk);
	if (this->_config->IsCombinedFk){
		this->_RunG28_CombinedFk(axis);

	}else{
		this->__HomeSingleAxis(axis);
	}

}


void CncSolutionBase::_running_G28(){
	// Serial.print("[Info] GobotHouseHardware::running_G28() is entering \n");

	if (this->__current_homer->IsTriged()){
		// End stop is trigered
		Serial.print("[Info] CncSolutionBase::_running_G28() Home sensor is trigered.  axis= " );
		Serial.println (this->_homing_axis_name);
		this->_board->RepportRamUsage();

		this->_board->cnc_mover->AllActuatorsStop();
		// The homed postion is a Inverse kinematic position for alpha, beta.
		this->_SetCurrentPositionAsHome(this->_homing_axis_name);
		IkPosition_AB ik_position;
		// Serial.print("222222222222222222");
		// this->_config->PrintOut("ggggggggggggggggggggggggggggggggggggggggggggggggggg");
		// if (this->_config->IsInverseKinematicHoimg){
		// 	Serial.print("\n   [Info] Trying to get home position from actuator position  ");
		// 	ik_position.alpha =  this->_fivebarMachine->Homed_position_alpha_in_rad;
		// 	ik_position.beta =  this->_fivebarMachine->Homed_position_beta_in_rad;
		// 	this->FK(&ik_position, &this->__current_fk_position);
		// 	// verify FK by IK()
		// 	IkPosition_AB verifying_ik;
		// 	Serial.print("\n\n  [Info] Please verify IK->FK->IK   ");
		// 	this->IK(&this->__current_fk_position, &verifying_ik);
		// }else{
		// 	Logger::Error("CncFiveBars::_running_G28()  Trying to get home position");
		// 	Serial.print(" with EEF-FK position is under construction");
		// 	Serial.println(FCBC_RESET);
		// }
		// //Copy current ik-position to motor-position.
		// this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(AXIS_ALPHA, ik_position.alpha);
		// this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(AXIS_BETA, ik_position.beta);
		
		this->State = CncState::IDLE;

	}else{
		// Endstop is not trigered

	}
}

void CncSolutionBase::_running_G1(){
    if (this->GetDistanceToTarget_IK() < 1){  // TODO: to determine g1 is finsied

      	this->State = CncState::IDLE;
		Serial.print("[Info] CncFiveBars::_running_G1() is finished. ");
    }
}
void CncSolutionBase::RunG4(Gcode* gcode){
	__g4_start_timestamp = micros();
	__g4_time_second = gcode->get_value('S');
}

void CncSolutionBase::__running_G4(){
	long delayed = (micros() - __g4_start_timestamp) / 1000 /1000;
	if (delayed >= __g4_time_second ){
		this->State = CncState::IDLE;
		return;
	}
}

void CncSolutionBase::RunM123(uint8_t eef_channel, uint8_t eef_action){
	Serial.print("[Debug] CncFiveBars::RunM123()  eef_action= ");
	Serial.println(eef_action);
	// uint8_t action_code = 1;
	this->__eef->Run(eef_action);
}

void CncSolutionBase::RunM84(){
	//TODO: CNC_AXIS_COUNT_IK,   vs CNC_AXIS_COUNT_FK
	for (int axis=0; axis<CNC_AXIS_COUNT; axis++){
		this->_board->EnableMotor(EnumAxis(axis), false);
	}
	// this->_board->EnableMotor(AXIS_ALPHA, false);
	// this->_board->EnableMotor(AXIS_BETA, false);
}
void CncSolutionBase::RunGcode(Gcode* gcode){
	std::string result;
	// if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND)){
	//   Serial.print("RunGcode()   OK or Unknown");
	//   return;
	// }
	bool debug = false;

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
			// if (gcode->has_letter('I')) this->_home_via_inverse_kinematic = true;
			if (home_axis == '+'){
				Serial.print("\n\n\n\n[Error] CncSolutionBase::RunGcode()  :");
				Serial.print(home_axis);

			}
			//TODO:  convert char to enum
			// this->ConvertToEnum(home_axis);
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
		EefAction action;
		switch (gcode->m){
		case 42:
			p_value =  gcode->get_value('P');
			s_value = gcode->get_value('S');
			this->RunM42(p_value, s_value);
		case 84:
			this->RunM84();
		case 114:
			// Get Current Position
			break;

		case 119:
			// Get Endstop Status
			result = GetHomeTrigerStateString();
			// this->commuDevice->OutputMessage(COMMU_OK);
			Serial.print(result.c_str());
			// this->commuDevice->WriteNotification(result);
			break;

		case 123:
			//M123 P=channel_index, S=Set EEF action			
			while (this->State != CncState::IDLE){
				this->SpinOnce();
			}
			p_value =  gcode->get_value('P');
			s_value = gcode->get_value('S');
			debug = true;
			if (debug){
				Serial.print("CncSolutionBase::RunGcode() For EEF_ACTION  M123 P= ");
				Serial.print(p_value);
				Serial.print("  S= ");
				Serial.print(s_value);
			}
			action = (EefAction)s_value;
			this->RunM123(p_value, s_value);
			break;

		case 280:
			// Set servo position  
			//	 	Pnnn Servo index
			// 		Snnn Angle or microseconds
			// Wait for all gcode, mcode is finished
			// Serial.println("M280 Started");
			while (this->State != CncState::IDLE){
				this->SpinOnce();
			}
			if (gcode->has_letter('P')) p_value = gcode->get_value('P');
			if (gcode->has_letter('S')) s_value = gcode->get_value('S');
			ledcWrite(p_value, s_value);   // from ledcWrite(ledChannel, dutyCycle);
			// this->commuDevice->OutputMessage(COMMU_OK);
			// this->commuDevice->WriteNotification("IDLE");
			break;
		case 996:
			// Do nothing. this should be the last gcode of a movement in transaction.
			// after MCU reset, This should be the first gcode it received, even the message queue is nothing.
			break;
		default:
			break;
		}
	}else{
		// this->commuDevice->OutputMessage("\n[Warning] CncSolutionBase::RunGcode()  Has NO letter 'G' or 'M'. ");
		// this->commuDevice->OutputMessage(gcode->get_command());
		// this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND);
	}
}


void CncSolutionBase::RunM42(uint8_t pin_number, uint8_t pin_value){
	digitalWrite(pin_number, pin_value);
}
