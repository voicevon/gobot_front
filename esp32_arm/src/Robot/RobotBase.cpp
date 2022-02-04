	#include "RobotBase.h"
	#include "MyLibs/MyFunctions.hpp"
	#include "HardwareSerial.h"

	void RobotBase::SpinOnce(){
	// commuDevice->SpinOnce();
	// Serial.print("[Debug] RobotBase::SpinOnce() is entering \n");
		// Serial.print(this->State);
		this->SpinOnce_BaseEnter();
		switch (this->State){
		case IDLE:
			break;
		case RUNNING_G4:
			this->__running_G4();
			break;
		case RUNNING_G1:
			this->_running_G1();
			break;
		case RUNNING_G28:
			this->_running_G28();
			break;
		default:
			Serial.print("[Warning] RobotBase::SpinOnce() Unknown current state: ");
			Serial.print(this->State);

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
			this->State = IDLE;
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
		char home_axis = 'X';
		switch (gcode->g){
		case 28:
			// G28: Home
			this->State = RUNNING_G28;
			if (gcode->has_letter('Y')) home_axis='Y';
			if (gcode->has_letter('Z')) home_axis='Z';
			if (gcode->has_letter('A')) home_axis='A';
			if (gcode->has_letter('B')) home_axis='B';
			if (gcode->has_letter('C')) home_axis='C';
			this->_home_as_inverse_kinematic = false;
			if (gcode->has_letter('I')) this->_home_as_inverse_kinematic = true;
			this->HomeSingleAxis(home_axis);
			// this->commuDevice->OutputMessage(COMMU_OK);  For calble-bot-corner, it should be 'Unknown Command'
			break;

		case 1:
			// G1 Move
			//TODO:  1. put position to movement queue. called "plan" in smoothieware? 
			//       2. send out OK.
			//       3. Set status to busy.
			//       4. Start Moving.
			this->State = RUNNING_G1;
			this->RunG1(gcode);
			this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 4:
			// G4 Dwell, Pause for a period of time.
			this->State = RUNNING_G4;
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
		uint8_t pin_id = 33;   //TODO: Make sure this is no harmful!
		uint8_t pin_value = 0;
		switch (gcode->m){
		case 42:
			pin_id =  gcode->get_value('P');
			pin_value = gcode->get_value('S');
			this->RunM42(pin_id, pin_value);
		case 119:
			// Get Endstop Status
			result = GetHomeTrigerStateString();
			this->commuDevice->OutputMessage(COMMU_OK);
			Serial.print(result.c_str());
			this->commuDevice->WriteNotification(result);
			break;
		case 114:
			// Get Current Position
			break;
		case 280:
			// Set servo position  
			//	 	Pnnn Servo index
			// 		Snnn Angle or microseconds
			// Wait for all gcode, mcode is finished
			// Serial.println("M280 Started");
			while (!this->State == IDLE){
			this->SpinOnce();
			}
			if (gcode->has_letter('P')) pin_id = gcode->get_value('P');
			if (gcode->has_letter('S')) pin_value = gcode->get_value('S');
			ledcWrite(pin_id, pin_value);   // from ledcWrite(ledChannel, dutyCycle);
			this->commuDevice->OutputMessage(COMMU_OK);
			this->commuDevice->WriteNotification("IDLE");
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
