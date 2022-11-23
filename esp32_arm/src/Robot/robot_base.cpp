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

	if (Queue_MoveBlock::Instance().BufferIsFull()){
		return;
	}

	if (Queue_LineSegment::Instance().BufferIsFull()){
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
		current_speed = speed;
	}else{
		speed = current_speed;
	}
	// // Assume G1-code want to update actuator directly, no need to do IK.
	FKPosition_XYZRPY target_fk;

	// IKPosition_abgdekl target_ik_ab;
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


// Before invoking this function. Make sure:
// 1. Queue_MoveBlock is not full
// 2. Queue_LineSegment is not full
void RobotBase::__RunGcode(Gcode* gcode){
	LineSegment* new_line = Queue_LineSegment::Instance().GetRoom();
	Queue_LineSegment::Instance().GetHeadLineSegment()->DeepCopyTo(new_line);
	// __current_line.DeepCopyTo(&new_line);
	MoveBlock* new_move_block = Queue_MoveBlock::Instance().GetRoom();
	Queue_MoveBlock::Instance().GetHead_MoveBlock()->DeepCopyTo(new_move_block);

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
			if (gcode->has_letter('X')) new_line->TargetPosition->X = gcode->get_value('X');
			if (gcode->has_letter('Y')) new_line->TargetPosition->Y = gcode->get_value('Y');
			if (gcode->has_letter('Z')) new_line->TargetPosition->Z = gcode->get_value('Z');
			if (gcode->has_letter('R')) new_line->TargetPosition->Roll = gcode->get_value('R');
			if (gcode->has_letter('P')) new_line->TargetPosition->Pitch = gcode->get_value('P');
			if (gcode->has_letter('W')) new_line->TargetPosition->Yaw = gcode->get_value('W');
			Queue_LineSegment::Instance().Deposit();
			// this->__planner.AppendLineSegment(new_line);   //TODO:: many lines ?

			break;

		case 6:
			// will directly put move_block to the queue.
			if (gcode->has_letter('a')) new_move_block->MoveBlocks[AXIS_ALPHA].TargetPosition = gcode->get_value('a');
			if (gcode->has_letter('b')) new_move_block->MoveBlocks[AXIS_BETA].TargetPosition = gcode->get_value('b');
			if (gcode->has_letter('g')) new_move_block->MoveBlocks[AXIS_GAMMA].TargetPosition = gcode->get_value('g');
			if (gcode->has_letter('d')) new_move_block->MoveBlocks[AXIS_DELTA].TargetPosition = gcode->get_value('d');
			if (gcode->has_letter('e')) new_move_block->MoveBlocks[AXIS_EPSILON].TargetPosition = gcode->get_value('e');
			if (gcode->has_letter('k')) new_move_block->MoveBlocks[AXIS_KAPPPA].TargetPosition = gcode->get_value('k');
			if (gcode->has_letter('l')) new_move_block->MoveBlocks[AXIS_LAMBDA].TargetPosition = gcode->get_value('l');
			Queue_MoveBlock::Instance().Deposit();
			break;

		case 90:
			// Absolute position
			// this->is_absolute_position = true;
			// this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 91:
			// Relative position
			// this->is_absolute_position = false;
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




