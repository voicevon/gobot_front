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
			// In case of these gocde in queue:    G1X123;  G4S5; G28X  what will hanppen 
			Logger::Print("RobotBase::SpinOnce()  G28_Runner is waiting ", 21);
			if (Queue_MoveBlock::Instance().BufferIsEmpty()){
				this->_g28_runner->Start();
				this->State = RobotState::G28_IS_RUNNING;
				Logger::Print("RobotBase::SpinOnce()  G28_Runner is started ", 22);
			}
			break;
		case RobotState::G28_IS_RUNNING:
			if(this->_g28_runner->IsDone()){
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
	MessageQueue::SingleMessage* message = this->_gcode_queue->FetchTailMessage(false);
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
	// __gcode = gcode;
	Logger::Debug("RobotBase::SpinOnce() has got command string ");
	Serial.println(str.c_str());
	// Logger::Print("RobotBase::SpinOnce() point", 6);
	// Logger::Print("gcode_command", gcode.get_command());

	if(gcode.has_g){
		this->__RunGcode(&gcode);
		this->_gcode_queue->FetchTailMessage(true);
	}else if(gcode.has_m){
		if (this->State == RobotState::IDLE_OR_ASYNC){
			bool is_finished = McodeOS::Instance().StartToRun(&gcode);   // DOING: How to run it async? assume the runner will take a long time.
			if (!is_finished){
				Logger::Warn("RobotBase::SpinOnce() ---- Run mCode background");
				Serial.print(gcode.get_command());
				this->State = RobotState::RUNNING_M_CODE;
			}
			this->_gcode_queue->FetchTailMessage(true);
		}
		// this->__RunMcode(&gcode);
	}
	// Logger::Print("RobotBase::SpinOnce() point", 99);

}

// void CncSolution_CoreAZ::RunG1(Gcode* gcode) {
bool RobotBase::_CutGcodeLine_ToSegmentQueue(Gcode* gcode){
	Serial.print("\n[Debug] CncSolution_CoreAZ::RunG1() is entering");
	Serial.print(gcode->get_command());
	// this->_cnc_board->EnableMotor(AXIS_ALPHA, true);
	// this->_cnc_board->EnableMotor(AXIS_BETA, true);
	// MoveBlock* mb = this->__queue_move_block->GetHeadMoveblock();
	// MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
	float speed = __planner.current_speed;
	if (gcode->has_letter('F')){
		speed = gcode->get_value('F');
		// this->_mover_base->SetEefSpeed(speed);
		// mb->MoveBlocks[AXIS_ALPHA].Speed = speed;
		// mb->MoveBlocks[AXIS_BETA].Speed = speed;
	}
	// // Assume G1-code want to update actuator directly, no need to do IK.
	// FkPosition_ZW target_fk_zw;
	// IkPosition_AlphaBeta target_ik_ab;
	// target_fk_zw.Z = this->__current_fk_position.Z;
	// target_fk_zw.W = this->__current_fk_position.W;
	// // target_ik_ab.alpha = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA);
	// // target_ik_ab.beta = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
	// bool do_ik=false;
	// if (gcode->has_letter(AXIS_ALPHA)) target_ik_ab.alpha = gcode->get_value(AXIS_ALPHA);
	// if (gcode->has_letter(AXIS_BETA)) target_ik_ab.beta = gcode->get_value(AXIS_BETA);

	// // If need IK, do it now.
	// if (gcode->has_letter('Z')) {
	// 	do_ik=true;
	// 	target_fk_zw.Z = gcode->get_value('Z');
	// }
	// if (gcode->has_letter('W')){
	// 	do_ik=true;
	// 	target_fk_zw.W = gcode->get_value('W');
	// }
	// if (do_ik) IK(&target_fk_zw,&target_ik_ab);

	// //Prepare actuator/driver to move to next point
	// // float motor_position[2];
	// // motor_position[0] = target_ik_ab.alpha;
	// // motor_position[1] = target_ik_ab.beta;
	// mb->MoveBlocks[AXIS_ALPHA].TargetPosition = target_ik_ab.alpha;
	// mb->MoveBlocks[AXIS_BETA].TargetPosition = target_ik_ab.beta;
	// // this->__queue_move_block->ForwardHead();
	// Queue_MoveBlock::Instance().Deposit();
	// //None blocking, move backgroundly.
	// // this->_mover_base->AllActuatorsMoveTo(true, motor_position);

	// if (true){
	// 	Serial.print("\n    [Debug] CncSolution_CoreAZ::RunG1()     (");
	// 	// Serial.print(this->objStepper_alpha->getPosition());
	// 	// Serial.print(this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA));
	// 	Serial.print(",");
	// 	// Serial.print(this->objStepper_beta->getPosition());
	// 	// Serial.print(this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA));
	// 	Serial.print(")   <-- from   alpha,beta   to -->  (");
	// 	Serial.print(target_ik_ab.alpha  );
	// 	Serial.print(" , ");
	// 	Serial.print(target_ik_ab.beta);
	// 	Serial.print(")");
	// }
}
void RobotBase::__RunGcode(Gcode* gcode){
	// char home_axis_name = '+';
	LineSegment line;
	// EnumAxis home_axis;
	Logger::Debug("RobotBase::__RunGcode()");
	switch (gcode->g){
		case 28:
			Logger::Print("RobotBase::__RunGcode() --- g28_runner->LinkGcode", 1);
			this->_g28_runner->LinkGcode(gcode);
			Logger::Print("RobotBase::__RunGcode() --- g28_runner->LinkGcode", 2);

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

// void RobotBase::__RunMcode(Gcode* gcode){
// 	bool debug = false;
// 		uint8_t p_value = 33;   //TODO: Make sure this is no harmful!
// 		uint8_t s_value = 0;
// 		float f_value = 0.0f;
// 		EefAction action;
// 		switch (gcode->m){
// 		case 123:
// 			//M123 P=channel_index, S=Set EEF action			
// 			while (this->State != RobotState::IDLE_OR_ASYNC){
// 				this->SpinOnce();
// 			}
// 			p_value =  gcode->get_value('P');
// 			s_value = gcode->get_value('S');
// 			debug = true;
// 			if (debug){
// 				Logger::Debug("RobotBase::RunGcode() For EEF_ACTION  M123 ");
// 				Logger::Print("P", p_value);
// 				Logger::Print("S", s_value);
// 			}
// 			action = (EefAction)s_value;
// 			this->RunM123(p_value, s_value);
// 			break;

// 		case 280:
// 			// Set servo position  
// 			//	 	Pnnn Servo index
// 			// 		Snnn Angle or microseconds
// 			// Wait for all gcode, mcode is finished
// 			// Serial.println("M280 Started");
// 			while (this->State != RobotState::IDLE_OR_ASYNC){
// 				this->SpinOnce();
// 			}
// 			if (gcode->has_letter('P')) p_value = gcode->get_value('P');
// 			if (gcode->has_letter('S')) s_value = gcode->get_value('S');
// 			ledcWrite(p_value, s_value);   // from ledcWrite(ledChannel, dutyCycle);
// 			// this->commuDevice->OutputMessage(COMMU_OK);
// 			// this->commuDevice->WriteNotification("IDLE");
// 			break;
// 		case 996:
// 			// Do nothing. this should be the last gcode of a movement in transaction.
// 			// after MCU reset, This should be the first gcode it received, even the message queue is nothing.
// 			break;
// 		default:
// 			break;
// 		}
// 	// Logger::Print("RobotBase::RunMcode() point ",99);

// }


