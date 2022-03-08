#include "RobotBase.h"
#include "MyLibs/MyFunctions.hpp"
#include "HardwareSerial.h"

// void RobotBase::FeedMessage(char* message, int length){
// 	Gcode gcode = Gcode(message);
// 	this->RunGcode(&gcode);
// }

void RobotBase::SpinOnce(){
// commuDevice->SpinOnce();
// Serial.print("[Debug] RobotBase::SpinOnce() is entering \n");
	// Serial.print(this->State);
	this->SpinOnce_BaseEnter();
	switch (this->State){
	case RobotState::IDLE:
		break;
	case RobotState::RUNNING_G4:
		this->__running_G4();
		break;
	case RobotState::RUNNING_G1:
		this->_running_G1();
		break;
	case RobotState::RUNNING_G28:
		this->_running_G28();
		break;
	default:
		Serial.print("[Warning] RobotBase::SpinOnce() Unknown current state: ");
		// Serial.print(this->State);

		break;
	}
	// if(commuDevice->HasNewChatting()){
	//   std::string command(commuDevice->ReadChatting());
	//   Serial.println ("    _base_spin_once()  new chatting");
	//   Serial.println(command.c_str());
	//   Gcode gCode = Gcode(command);   //Risk for not releasing memory ?
	//   this->RunGcode(&gCode);
	// }
	this->SpinOnce_BaseExit();
}

void RobotBase::RunG4(Gcode* gcode){
	__g4_start_timestamp = micros();
	__g4_time_second = gcode->get_value('S');
}

void RobotBase::__running_G4(){
	long delayed = (micros() - __g4_start_timestamp) / 1000 /1000;
	if (delayed >= __g4_time_second ){
		this->State = RobotState::IDLE;
		return;
	}
}

void RobotBase::RunGcode(Gcode* gcode){
	std::string result;
	// if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND)){
	//   Serial.print("RunGcode()   OK or Unknown");
	//   return;
	// }

	if(gcode->has_g){
		char home_axis = '+';
		switch (gcode->g){
		case 28:
			// G28: Home
			this->State = RobotState::RUNNING_G28;
			if (gcode->has_letter('X')) home_axis='X';
			if (gcode->has_letter('Y')) home_axis='Y';
			if (gcode->has_letter('Z')) home_axis='Z';
			if (gcode->has_letter('A')) home_axis='A';
			if (gcode->has_letter('B')) home_axis='B';
			if (gcode->has_letter('C')) home_axis='C';
			if (gcode->has_letter('W')) home_axis='W';
			this->_home_as_inverse_kinematic = false;
			if (gcode->has_letter('I')) this->_home_as_inverse_kinematic = true;
			if (home_axis == '+'){
				Serial.print("\n\n\n\n[Error] RobotBase::RunGcode()  :");
				Serial.print(home_axis);

			}
			this->HomeSingleAxis(home_axis);
			// this->commuDevice->OutputMessage(COMMU_OK);  For calble-bot-corner, it should be 'Unknown Command'
			break;

		case 1:
			// G1 Move
			//TODO:  1. put position to movement queue. called "plan" in smoothieware? 
			//       2. send out OK.
			//       3. Set status to busy.
			//       4. Start Moving.
			this->State = RobotState::RUNNING_G1;
			this->RunG1(gcode);
			this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 4:
			// G4 Dwell, Pause for a period of time.
			this->State = RobotState::RUNNING_G4;
			this->RunG4(gcode);
			break;
		case 6:
			this->RunG6(gcode);
			this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 90:
			// Absolute position
			this->is_absolute_position = true;
			this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 91:
			// Relative position
			this->is_absolute_position = false;
			this->commuDevice->OutputMessage(COMMU_OK);
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
			this->commuDevice->OutputMessage(COMMU_OK);
			Serial.print(result.c_str());
			this->commuDevice->WriteNotification(result);
			break;

		case 123:
			//M123 P=channel_index, S=Set EEF action			
			while (this->State != RobotState::IDLE){
				this->SpinOnce();
			}
			p_value =  gcode->get_value('P');
			s_value = gcode->get_value('S');
			Serial.print("RobotBase::RunGcode() For EEF_ACTION  M123 P= ");
			Serial.print(p_value);
			Serial.print("  S= ");
			Serial.print(s_value);
			action = (EefAction)s_value;
			this->RunM123(p_value, action);
			break;

		case 280:
			// Set servo position  
			//	 	Pnnn Servo index
			// 		Snnn Angle or microseconds
			// Wait for all gcode, mcode is finished
			// Serial.println("M280 Started");
			while (this->State != RobotState::IDLE){
				this->SpinOnce();
			}
			if (gcode->has_letter('P')) p_value = gcode->get_value('P');
			if (gcode->has_letter('S')) s_value = gcode->get_value('S');
			ledcWrite(p_value, s_value);   // from ledcWrite(ledChannel, dutyCycle);
			this->commuDevice->OutputMessage(COMMU_OK);
			this->commuDevice->WriteNotification("IDLE");
			break;
		case 996:
			// Do nothing. this should be the last gcode of a movement in transaction.
			// after MCU reset, This should be the first gcode it received, even the message queue is nothing.
			break;
		default:
			break;
		}
	}else{
		this->commuDevice->OutputMessage("\n[Warning] RobotBase::RunGcode()  Has NO letter 'G' or 'M'. ");
		this->commuDevice->OutputMessage(gcode->get_command());
		this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND);
	}
}


void RobotBase::RunM42(uint8_t pin_number, uint8_t pin_value){
	digitalWrite(pin_number, pin_value);
}
