#include "robot_base.h"
#include "MyLibs/MyFunctions.hpp"
#include "HardwareSerial.h"
#include "MyLibs/message_queue.h"
#include "CNC/arm_solution/arm_solution_base.h"


void RobotBase::SpinOnce(){
	// Logger::Debug("RobotBase::SpinOnce()");
	this->_mover->SpinOnce();
	// Logger::Print("RobotBase::SpinOnce() point", 2);

	switch (this->State){
		case RobotState::G4_IS_SYNCING:
			//todo:  when buffer is empty,  a gcode is still runnning.
			// Correct way is:  buffer is empey, and Mover is stoped.
			// if (this->__queue_move_block.BufferIsEmpty()){
			Logger::Print("RobotBase::SpinOnce() G4_Runner is Waiting. ", 41);
			if (Queue_MoveBlock::Instance().BufferIsEmpty()){
				this->__g4_runner.Start();
				this->State = RobotState::G4_IS_RUNNING;
				Logger::Print("RobotBase::SpinOnce()  G4_Runner is started", 42);
			}
			break;
		case RobotState::G4_IS_RUNNING:
			if(this->__g4_runner.IsDone()){
				this->State =RobotState::IDLE_OR_ASYNC;
				Logger::Print("RobotBase::SpinOnce()  G4_Runner is over", 43);
			}
			break;
		case RobotState::G28_IS_SYNCING:
			Logger::Print("RobotBase::SpinOnce()  G28_Runner is waiting ", 21);
			// if (this->__queue_move_block.BufferIsEmpty()){
			if (Queue_MoveBlock::Instance().BufferIsEmpty()){
				this->__g28_runner->Start();
				this->State = RobotState::G28_IS_RUNNING;
				Logger::Print("RobotBase::SpinOnce()  G28_Runner is started ", 22);
			}
			break;
		case RobotState::G28_IS_RUNNING:
			if(this->__g28_runner->IsDone()){
				this->State = RobotState::IDLE_OR_ASYNC;
				Logger::Print("RobotBase::SpinOnce() point G28_Runner is over ", 23);
			}
			break;

		default:
			// Logger::Print("RobotBase::SpinOnce() point", 29);
			break;
	}
	if (this->State != RobotState::IDLE_OR_ASYNC ) {
		// Logger::Print("RobotBase::SpinOnce() point", 91);
		return;
	}
	if (this->_gcode_queue->BufferIsEmpty()){
		// Logger::Print("RobotBase::SpinOnce() point", 92);
		return;
	}
	if (!this->__planner.IsPlanable()){
		Logger::Print("RobotBase::SpinOnce() Planner can not go on,  queue might be full(or almost full).", 93);
		return;
	}
	Logger::Print("RobotBase::SpinOnce() point", 3);

	// Check gcode queue, if there is gcode to be run.
	MessageQueue::SingleMessage* message = this->_gcode_queue->FetchTailMessage();
	Logger::Print("RobotBase::SpinOnce() point", 4);
	if (message == NULL){
		Logger::Error("\n\n\n [Error] RobotBase::SpinOnce() tail_message is null. \n\n ");
		return;
	}

	bool debug = false;
	if (debug){
		Logger::Debug("[Info] RobotBase::SpinOnce()  Going to run next gcode   ===> ");
		Serial.print(message->payload);
		Serial.println(" ");
	}
	
	// type convert   from char* to std::string
	char* p = &message->payload[0];
	std::string str = std::string(p);
	// feed std::string to Gcode constructor.
	Gcode gcode = Gcode(str);
	Logger::Debug("RobotBase::SpinOnce() has got command string ");
	Serial.println(str.c_str());
	Logger::Print("RobotBase::SpinOnce() point", 6);
	Logger::Print("gcode_command", gcode.get_command());

	if(gcode.has_g){
		this->__RunGcode(&gcode);
	}else if(gcode.has_m){
		this->__RunMcode(&gcode);
	}
	// Logger::Print("RobotBase::SpinOnce() point", 99);

}

void RobotBase::__RunGcode(Gcode* gcode){
	// char home_axis_name = '+';
	LineSegment line;
	// EnumAxis home_axis;

	switch (gcode->g){
		case 28:
			this->__g28_runner->LinkGcode(gcode);

			//TODO:  convert char to enum
			// this->RunG28(this->ConvertToEnum(home_axis));
			// home_axis =  this->_arm_solution->ConvertToEnum(home_axis_name);
			this->State = RobotState::G28_IS_SYNCING;
			break;
		case 4:
			// G4 Dwell, Pause for a period of time.
			this->__g4_runner.LinkGcode(gcode);
			this->State = RobotState::G4_IS_SYNCING;
			break;
						
		case 1:
			// G1 Move
			//TODO:  1. put position to movement queue. called "plan" in smoothieware? 
			//       2. send out OK.
			//       3. Set status to busy.
			//       4. Start Moving.
			this->__planner.__arm_solution->_CutGcodeLine_ToSegmentQueue(gcode);
			this->__planner.AppendLineSegment(&line);   //TODO:: many lines ?

			// this->commuDevice->OutputMessage(COMMU_OK);
			break;

		// case 6:
			// this->RunG6(gcode);
			// this->commuDevice->OutputMessage(COMMU_OK);
			// break;
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
		case 92:
			//Set Position     G92 X10 E90
			break;
		default:
			break;
	}
	// Logger::Print("RobotBase::RunMcode() point ",99);
		
}

void RobotBase::__RunMcode(Gcode* gcode){
	bool debug = false;
		uint8_t p_value = 33;   //TODO: Make sure this is no harmful!
		uint8_t s_value = 0;
		float f_value = 0.0f;
		EefAction action;
		McodeRunnerBase* mr;
		switch (gcode->m){
		case 42:
			McodeRunners::Run(gcode);
			// mr->Run(gcode);
			// McodeRunners::GetRunner(42)->Run(gcode);
			
			// this->__m42_runner.Run(gcode);
			// p_value =  gcode->get_value('P');
			// s_value = gcode->get_value('S');
			// this->Run_M42_OutputGpio(p_value, s_value);
			break;
		case 84:
			// McodeRunners::GetRunner(84)->Run(gcode);
			// this->__m84_runner.Run(gcode);
			break;
		case 114:
			// Get Current Position
			break;

		case 119:
			// Get Endstop Status
			// result = GetHomeTrigerStateString();
			// this->commuDevice->OutputMessage(COMMU_OK);
			// Serial.print(result.c_str());
			// this->commuDevice->WriteNotification(result);
			break;

		case 123:
			//M123 P=channel_index, S=Set EEF action			
			while (this->State != RobotState::IDLE_OR_ASYNC){
				this->SpinOnce();
			}
			p_value =  gcode->get_value('P');
			s_value = gcode->get_value('S');
			debug = true;
			if (debug){
				Logger::Debug("RobotBase::RunGcode() For EEF_ACTION  M123 ");
				Logger::Print("P", p_value);
				Logger::Print("S", s_value);
			}
			action = (EefAction)s_value;
			this->RunM123(p_value, s_value);
			break;

		case 130:
			// this->__m130_runner.Run(gcode);
			McodeRunners::getInstance().Run(gcode);
			break;

		case 141:
			break;

		case 280:
			// Set servo position  
			//	 	Pnnn Servo index
			// 		Snnn Angle or microseconds
			// Wait for all gcode, mcode is finished
			// Serial.println("M280 Started");
			while (this->State != RobotState::IDLE_OR_ASYNC){
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
	// Logger::Print("RobotBase::RunMcode() point ",99);

}


// void RobotBase::RunM123(uint8_t eef_channel, uint8_t eef_action){
// 	Logger::Debug("RobotBase::RunM123()");
// 	Logger::Print("eef_action", eef_action);
// 	// uint8_t action_code = 1;
// 	this->__eef->PrintOut();
// 	this->__eef->Run(eef_action);
// }

// void RobotBase::RunM84(){
// 	//TODO: CNC_AXIS_COUNT_IK,   vs CNC_AXIS_COUNT_FK
// 	for (int axis=0; axis<CNC_AXIS_COUNT; axis++){
// 		this->_cnc_board->EnableMotor(EnumAxis(axis), false);
// 	}
// }

void RobotBase::Run_M42_OutputGpio(uint8_t pin_number, uint8_t pin_value){
	digitalWrite(pin_number, pin_value);
}
