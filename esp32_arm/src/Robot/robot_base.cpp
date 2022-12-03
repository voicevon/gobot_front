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
			// Logger::Print("RobotBase::SpinOnce()  G4_Runner is Waiting. ", 41);
			if (! Queue_MoveBlock::Instance().BufferIsEmpty()){
				return;
			}
			// Logger::Print("RobotBase::SpinOnce()  G4_Runner is Waiting. ", 42);

			if (CncActuator_List::Instance().HasMovingActuator())
				return;
			// Logger::Print("RobotBase::SpinOnce()  G4_Runner is starting. ", 43);
			this->__g4_runner.Start();
			this->State = RobotState::G4_IS_RUNNING;
			// Logger::Print("RobotBase::SpinOnce()  G4_Runner is started", 44);
			break;
			
		case RobotState::G4_IS_RUNNING:
			if(this->__g4_runner.IsDone()){
				this->State =RobotState::IDLE_OR_ASYNC;
				// Logger::Print("RobotBase::SpinOnce()  G4_Runner is over", 45);
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
				// IKPosition_abgdekl homed_ik_position;
				// FKPosition_XYZRPY homed_fk_position;
				// this->__planner.arm_solution->GetRealTimePosition(&homed_fk_position);
				// char homing_axis_name = this->_g28_runner->GetHomingAxisName();
				// if (CncAxis::IsCncAxis_FkName(homing_axis_name)){
				// 	this->__planner.arm_solution->SetCurrentPosition(&homed_fk_position);
				// }else if (CncAxis::IsActuator_IkName(homing_axis_name)){
				// 	this->__planner.arm_solution->IK(&homed_fk_position, &homed_ik_position);
				// 	// homed_ik_position.alpha = this->_g28_runner->GetTriggerPosition();
				// 	homed_ik_position.Positions[AXIS_ALPHA] = this->_g28_runner->GetTriggerPosition();
				// 	this->__planner.arm_solution->SetCurrentPosition(&homed_ik_position);
				// }
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
	// Logger::Print("RobotBase::SpinOnce() point", 3);

	// Check gcode queue, if there is gcode to be run.
	MessageQueue::SingleMessage* message = this->_gcode_queue->FetchTailMessage(false);
	// Logger::Print("RobotBase::SpinOnce() point", 4);
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
	}
	// Logger::Print("RobotBase::SpinOnce() point", 99);

}


// Before invoking this function. Make sure:
// 1. Queue_MoveBlock is not full
// 2. Queue_LineSegment is not full
void RobotBase::__RunGcode(Gcode* gcode){
	// Logger::Info("RobotBase::__RunGcode()");
	LineSegment* new_line = Queue_LineSegment::Instance().GetRoom();
	Queue_LineSegment::Instance().GetHeadLineSegment()->DeepCopyTo(new_line);
	MoveBlock* new_move_block = Queue_MoveBlock::Instance().GetRoom();
	Queue_MoveBlock::Instance().GetHead_MoveBlock()->DeepCopyTo(new_move_block);
	FKPosition_XYZRPY new_fk_position;
	IKPosition_abgdekl new_ik_position;
	LineSegment middle_kinematic_line;
	
	if (gcode->has_letter('F')) __newest_line_speed = gcode->get_value('F');
	// Logger::Print("RobotBase::__RunGcode() point", 51);
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
			// G1 Move. will follow a Forward kinematic poisition.
			// The position will be put in to Queue_LineSegment, Then convert to Queue_MoveBlock by Planner
			if (gcode->has_letter('X')) new_line->TargetPosition.X = gcode->get_value('X');
			if (gcode->has_letter('Y')) new_line->TargetPosition.Y = gcode->get_value('Y');
			if (gcode->has_letter('Z')) new_line->TargetPosition.Z = gcode->get_value('Z');
			if (gcode->has_letter('R')) new_line->TargetPosition.Roll = gcode->get_value('R');
			if (gcode->has_letter('P')) new_line->TargetPosition.Pitch = gcode->get_value('P');
			if (gcode->has_letter('W')) new_line->TargetPosition.Yaw = gcode->get_value('W');
			new_line->IsMiddleKinematicPosition = false;
			new_line->Speed_mm_per_second = __newest_line_speed;
			__planner.ConvertLineSegment_AppendMoveBlocks(new_line);
			Queue_LineSegment::Instance().Deposit();
			// this->__planner.AppendLineSegment(new_line);   //TODO:: many lines ?
			break;
		case 5:
			// G5 Move. will follow a Middle kinematic poisition.
			// The position will be put in to Queue_LineSegment, Then Convert to Queue_moveBlock by Planner.
			if (gcode->has_letter('X')) middle_kinematic_line.TargetPosition.X = gcode->get_value('X');
			if (gcode->has_letter('Y')) middle_kinematic_line.TargetPosition.Y = gcode->get_value('Y');
			if (gcode->has_letter('Z')) middle_kinematic_line.TargetPosition.Z = gcode->get_value('Z');
			if (gcode->has_letter('R')) middle_kinematic_line.TargetPosition.Roll = gcode->get_value('R');
			if (gcode->has_letter('P')) middle_kinematic_line.TargetPosition.Pitch = gcode->get_value('P');
			if (gcode->has_letter('W')) middle_kinematic_line.TargetPosition.Yaw = gcode->get_value('W');
			new_line->IsMiddleKinematicPosition = true;
			new_line->Speed_mm_per_second = __newest_line_speed;
			this->__planner.arm_solution->MK_to_FK(&middle_kinematic_line.TargetPosition , &new_line->TargetPosition);
			__planner.ConvertLineSegment_AppendMoveBlocks(new_line);
			Queue_LineSegment::Instance().Deposit();
			break;

		case 6:
			// G6 Move.will directly put move_block to the queue.
			if (gcode->has_letter('a')) new_move_block->MoveBlocks[AXIS_ALPHA].TargetPosition = gcode->get_value('a');
			if (gcode->has_letter('b')) new_move_block->MoveBlocks[AXIS_BETA].TargetPosition = gcode->get_value('b');
			if (gcode->has_letter('g')) new_move_block->MoveBlocks[AXIS_GAMMA].TargetPosition = gcode->get_value('g');
			if (gcode->has_letter('d')) new_move_block->MoveBlocks[AXIS_DELTA].TargetPosition = gcode->get_value('d');
			if (gcode->has_letter('e')) new_move_block->MoveBlocks[AXIS_EPSILON].TargetPosition = gcode->get_value('e');
			if (gcode->has_letter('k')) new_move_block->MoveBlocks[AXIS_KAPPPA].TargetPosition = gcode->get_value('k');
			if (gcode->has_letter('l')) new_move_block->MoveBlocks[AXIS_LAMBDA].TargetPosition = gcode->get_value('l');
			for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
				new_move_block->MoveBlocks[a].Speed = __newest_line_speed;
				new_move_block->MoveBlocks[a].Acceleration = 100;
			}
			// new_move_block->PrintOut("From RobotBase::__RunGcode()");
			Queue_MoveBlock::Instance().Deposit();
			// Update Current FK position 
			new_move_block->DeepCopyToIkPosition(&new_ik_position);
			__planner.arm_solution->FK(&new_ik_position, &new_fk_position);
			new_line->DeepCopyFromFkPosition(&new_fk_position);
			new_line->Speed_mm_per_second = __newest_line_speed;   //TODO: for next fk gcode usage. should be A:) default speed,  B:) follow previous speed
			Queue_LineSegment::Instance().Deposit();
			break;
		case 7:
			// G7 Move. will follow a Middle kinematic poisition.
			// The position will be put into Queue_moveBlock after convertion of MK->IK.
			if (gcode->has_letter('X')) middle_kinematic_line.TargetPosition.X = gcode->get_value('X');
			if (gcode->has_letter('Y')) middle_kinematic_line.TargetPosition.Y = gcode->get_value('Y');
			if (gcode->has_letter('Z')) middle_kinematic_line.TargetPosition.Z = gcode->get_value('Z');
			if (gcode->has_letter('R')) middle_kinematic_line.TargetPosition.Roll = gcode->get_value('R');
			if (gcode->has_letter('P')) middle_kinematic_line.TargetPosition.Pitch = gcode->get_value('P');
			if (gcode->has_letter('W')) middle_kinematic_line.TargetPosition.Yaw = gcode->get_value('W');
			new_line->IsMiddleKinematicPosition = true;
			new_line->Speed_mm_per_second = __newest_line_speed;
			this->__planner.arm_solution->MK_to_Ik(&middle_kinematic_line.TargetPosition, &new_ik_position);
			Queue_MoveBlock::Instance().Deposit();
			// Update Current FK position 
			new_move_block->DeepCopyToIkPosition(&new_ik_position);
			for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
				new_move_block->MoveBlocks[a].TargetPosition = new_ik_position.Positions[a];
				new_move_block->MoveBlocks[a].Speed = __newest_line_speed;
				new_move_block->MoveBlocks[a].Acceleration = 100;
			}
			__planner.arm_solution->FK(&new_ik_position, &new_fk_position);
			new_line->DeepCopyFromFkPosition(&new_fk_position);
			new_line->Speed_mm_per_second = __newest_line_speed;   //TODO: for next fk gcode usage. should be A:) default speed,  B:) follow previous speed
			Queue_LineSegment::Instance().Deposit();
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




