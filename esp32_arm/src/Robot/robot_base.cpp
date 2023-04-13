#include "robot_base.h"
#include "MyLibs/MyFunctions.hpp"
#include "HardwareSerial.h"
// #include "MyLibs/basic/queue/message_queue.h"
// #include "Mqtt/mqtt_message_queue.h"
#include "CNC/arm_solution/arm_solution_base.h"


void RobotBase::SpinOnce(){
	Logger::Debug("RobotBase::SpinOnce()");
	this->_mover->SpinOnce();
	Logger::Print("RobotBase::SpinOnce() point", 2);
	switch (this->State){
		case RobotState::MCODE_IS_SYNCING:
			Logger::Print("RobotBase::SpinOnce()  G4_Runner is Waiting. ", 41);
			if (! gs_MoveBlock_Queue::Instance().BufferIsEmpty()){
				return;
			}
			Logger::Print("RobotBase::SpinOnce()  G4_Runner is Waiting. ", 42);

			if (gs_CncActuator_List::Instance().HasMovingActuator())
				return;
			Logger::Print("RobotBase::SpinOnce()  G4_Runner is starting. ", 43);
			this->State = RobotState::MCODE_IS_RUNNING;
			break;

		case RobotState::MCODE_IS_RUNNING:
			if(McodeOS::Instance().RunnerLoop()){
				// Finished running current mcode.
				this->State = RobotState::IDLE_OR_ASYNC;
			}
			break;

		case RobotState::G4_IS_SYNCING:
			// Logger::Print("RobotBase::SpinOnce()  G4_Runner is Waiting. ", 41);
			if (! gs_MoveBlock_Queue::Instance().BufferIsEmpty()){
				return;
			}
			Logger::Print("RobotBase::SpinOnce()  G4_Runner is Waiting. ", 42);

			if (gs_CncActuator_List::Instance().HasMovingActuator())
				return;
			Logger::Print("RobotBase::SpinOnce()  G4_Runner is starting. ", 43);
			this->__g4_runner.Start();
			this->State = RobotState::G4_IS_RUNNING;
			Logger::Print("RobotBase::SpinOnce()  G4_Runner is started", 44);
			break;
			
		case RobotState::G4_IS_RUNNING:
			if(this->__g4_runner.IsDone()){
				this->State =RobotState::IDLE_OR_ASYNC;
				Logger::Print("RobotBase::SpinOnce()  G4_Runner is over", 45);
			}
			break;

		case RobotState::G28_IS_SYNCING:
			// In case of these gocde in queue:    G1X123;  G4S5; G28X  what will hanppen 
			Logger::Print("RobotBase::SpinOnce()  G28_Runner is waiting ", 21);
			if (gs_MoveBlock_Queue::Instance().BufferIsEmpty()){
				this->_g28_runner->Start();
				this->State = RobotState::G28_IS_RUNNING;
				// Logger::Print("RobotBase::SpinOnce()  G28_Runner is started ", 22);
			}
			break;
		case RobotState::G28_IS_RUNNING:
			if(this->_g28_runner->IsDone()){
				this->State = RobotState::IDLE_OR_ASYNC;
				Logger::Print("RobotBase::SpinOnce() point G28_Runner is over  ", 23);
			}
			break;

		default:
			// Logger::Print("RobotBase::SpinOnce() point", 29);
			break;
	}

	if (this->State != RobotState::IDLE_OR_ASYNC ) {
		Logger::Print("RobotBase::SpinOnce() point 91", 91);
		return;
	}

	if (gs_MoveBlock_Queue::Instance().BufferIsFull()){
		return;
	}

	if (gs_LineSegment_Queue::Instance().BufferIsFull()){
		return;
	}

	if (this->_gcode_queue.BufferIsEmpty()){
		// Logger::Print("RobotBase::SpinOnce() point", 92);
		return;
	}
	if (!this->__planner.IsPlanable()){
		Logger::Print("RobotBase::SpinOnce() Planner can not go on,  queue might be full(or almost full).", 93);
		return;
	}
	Logger::Print("RobotBase::SpinOnce() point  Going to run next gcode ...", 3);

	// Check gcode queue, if there is gcode to be run.
	GcodeText* gcode_text = this->_gcode_queue.WithdrawTailElement();
	// Logger::Print("RobotBase::SpinOnce() point", 4);
	if (gcode_text == NULL){
		Logger::Error("\n\n\n [Error] RobotBase::SpinOnce() tail_message is null. 991. \n\n ");
		return;
	}

	bool debug = true;
	if (debug){
		Logger::Debug("RobotBase::SpinOnce()  Going to run next gcode   >>>");
		Serial.print(gcode_text->bytes);
		Serial.println("<<<");
	}
	
	// type convert   from char* to std::string
	// char* p = (char*) (&message->payload[0]);
	// std::string str = std::string(p);
	// feed std::string to Gcode constructor.
	GcodeHelper gcode_helper = GcodeHelper(gcode_text->bytes);
	// Logger::Debug("RobotBase::SpinOnce() has got command string ");
	// Serial.println(str.c_str());
	// Logger::Print("RobotBase::SpinOnce() point", 6);
	Logger::Print("gcode_command", gcode_helper.get_command());
	if(gcode_helper.has_g){
		// Logger::Print("RobotBase::SpinOnce()   point 61", 61);
		this->__RunGcode(gcode_text);
		// Logger::Print("RobotBase::SpinOnce()   point 62", 61);
		this->_gcode_queue.WithdrawTailElement();
		Logger::Print("RobotBase::SpinOnce()   point 69", 61);
	}else if(gcode_helper.has_m){
		McodeOS::Instance().SetupRunner(gcode_text);
		this->State = RobotState::MCODE_IS_SYNCING;
		this->_gcode_queue.WithdrawTailElement();
	}else{
		Logger::Warn("RobotBase::SpinOnce() ---- Unknown command, Ignored.");
		Serial.println(gcode_text->bytes);
		this->_gcode_queue.WithdrawTailElement();

	}
	// Logger::Print("RobotBase::SpinOnce() point 99", 99);

}


// Before invoking this function. Make sure:
// 1. gs_MoveBlock_Queue is not full
// 2. gs_LineSegment_Queue is not full
void RobotBase::__RunGcode(GcodeText* gcode_text){
	GcodeHelper gcode_helper = GcodeHelper(gcode_text->bytes);
	GcodeHelper* gcode = &gcode_helper;
	static float __newest_line_speed = 100;
	// Logger::Info("RobotBase::__RunGcode()");
	LineSegment* new_line = gs_LineSegment_Queue::Instance().GetRoom_ForDeposit();
	new_line->DeepCopyTo_TargetPosition_fk(__planner.arm_solution->GetCurrentPosition_Fk());
	if (gcode->has_letter('F')) {
		__newest_line_speed = gcode->get_value('F');
	}
	new_line->Speed_mm_per_second = __newest_line_speed;
	// Logger::Print("RobotBase::__RunGcode      point", 11);
	MoveBlock* new_move_block = gs_MoveBlock_Queue::Instance().GetRoom_ForDeposit();
	// Logger::Print("RobotBase::__RunGcode      point", 12);
	//TODO:    This is wrong for the very first moveblock after MCU is reset.
	// gs_MoveBlock_Queue::Instance().GetDepositHeadElement()->DeepCopyTo(new_move_block);

	// Logger::Print("RobotBase::__RunGcode      point", 19);
	FKPosition_XYZRPW new_fk_position;
	IKPosition_abgdekl new_ik_position;
	LineSegment middle_kinematic_line;
	FKPosition_XYZRPW* current_position_FK;
	
	// Logger::Print("RobotBase::__RunGcode() point", 51);
	switch (gcode->g){
		case 28:
			// Logger::Print("RobotBase::__RunGcode() G28  point", 281);
			this->_g28_runner->LinkGcode(gcode_text);
			// Logger::Print("RobotBase::__RunGcode() G28  point", 282);
			this->State = RobotState::G28_IS_SYNCING;
			// Logger::Print("RobotBase::__RunGcode() G28  point", 283);
			break;
		case 4:
			// G4 Dwell, Pause for a period of time.
			this->__g4_runner.LinkGcode(gcode_text);
			this->State = RobotState::G4_IS_SYNCING;
			break;
						
		case 1:
			// G1 Move. will follow a Forward kinematic poisition.
			// The position will be put in to gs_LineSegment_Queue, Then convert to gs_MoveBlock_Queue by Planner
			if (gcode->has_letter('X')) new_line->TargetPosition.X = gcode->get_value('X');
			if (gcode->has_letter('Y')) new_line->TargetPosition.Y = gcode->get_value('Y');
			if (gcode->has_letter('Z')) new_line->TargetPosition.Z = gcode->get_value('Z');
			if (gcode->has_letter('R')) new_line->TargetPosition.Roll = gcode->get_value('R');
			if (gcode->has_letter('P')) new_line->TargetPosition.Pitch = gcode->get_value('P');
			if (gcode->has_letter('W')) new_line->TargetPosition.Yaw = gcode->get_value('W');
			// Logger::Print("RobotBase::__RunGcode() G1      point", 61);
			new_line->IsMiddleKinematicPosition = false;
			// new_line->PrintOUt("caller is RobotBase::__RunGcode() G1");
			// Logger::Print("RobotBase::__RunGcode      point", 52);
			__planner.ConvertLineSegment_AppendMoveBlocks(new_line);
			// Logger::Print("RobotBase::__RunGcode      point", 53);
			// gs_LineSegment_Queue::Instance().PrintOut("caller is __RunGcode()  531");
			// gs_LineSegment_Queue::Instance().Deposit();   //TODO:  IS this necessary
			// Logger::Print("RobotBase::__RunGcode      point", 59);
			break;
		case 5:
			// G5 Move. will follow a Middle kinematic poisition.
			// The position will be put in to gs_LineSegment_Queue, Then Convert to Queue_moveBlock by Planner.
			if (gcode->has_letter('X')) middle_kinematic_line.TargetPosition.X = gcode->get_value('X');
			if (gcode->has_letter('Y')) middle_kinematic_line.TargetPosition.Y = gcode->get_value('Y');
			if (gcode->has_letter('Z')) middle_kinematic_line.TargetPosition.Z = gcode->get_value('Z');
			if (gcode->has_letter('R')) middle_kinematic_line.TargetPosition.Roll = gcode->get_value('R');
			if (gcode->has_letter('P')) middle_kinematic_line.TargetPosition.Pitch = gcode->get_value('P');
			if (gcode->has_letter('W')) middle_kinematic_line.TargetPosition.Yaw = gcode->get_value('W');
			new_line->IsMiddleKinematicPosition = true;
			this->__planner.arm_solution->MK_to_FK(&middle_kinematic_line.TargetPosition , &new_line->TargetPosition);
			// new_line->PrintOUt("caller is RobotBase::__RunGcode() G5");
			__planner.ConvertLineSegment_AppendMoveBlocks(new_line);
			gs_LineSegment_Queue::Instance().Deposit();
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
			for(int a=0; a<gs_CncActuator_List::Instance().GetItemsCount(); a++){
				new_move_block->MoveBlocks[a].Speed = __newest_line_speed;
				new_move_block->MoveBlocks[a].Acceleration = 100;
			}
			// new_move_block->PrintOut("From RobotBase::__RunGcode()");
			gs_MoveBlock_Queue::Instance().Deposit();
			// Update Current FK position 
			new_move_block->DeepCopyToIkPosition(&new_ik_position);
			__planner.arm_solution->IK_to_FK(&new_ik_position, &new_fk_position);

			new_line->DeepCopyTo_TargetPosition_fk(&new_fk_position);  //??
			gs_LineSegment_Queue::Instance().Deposit();             //??
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
			this->__planner.arm_solution->MK_to_Ik(&middle_kinematic_line.TargetPosition, &new_ik_position);
			gs_MoveBlock_Queue::Instance().Deposit();
			// Update Current FK position 
			new_move_block->DeepCopyToIkPosition(&new_ik_position);
			for(int a=0; a<gs_CncActuator_List::Instance().GetItemsCount(); a++){
				new_move_block->MoveBlocks[a].TargetPosition = new_ik_position.Positions[a];
				new_move_block->MoveBlocks[a].Speed = __newest_line_speed;
				new_move_block->MoveBlocks[a].Acceleration = 100;
			}
			__planner.arm_solution->IK_to_FK(&new_ik_position, &new_fk_position);
			new_line->DeepCopyTo_TargetPosition_fk(&new_fk_position);
			gs_LineSegment_Queue::Instance().Deposit();
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
			current_position_FK =  __planner.arm_solution->GetCurrentPosition_Fk();
			new_fk_position.X = current_position_FK->X;
			new_fk_position.Y = current_position_FK->Y;
			new_fk_position.Z = current_position_FK->Z;
			new_fk_position.Roll = current_position_FK->Roll;
			new_fk_position.Pitch = current_position_FK->Pitch;
			new_fk_position.Yaw = current_position_FK->Yaw;
			if (gcode->has_letter('X')) new_fk_position.X = gcode->get_value('X');
			if (gcode->has_letter('Y')) new_fk_position.Y = gcode->get_value('Y');
			if (gcode->has_letter('Z')) new_fk_position.Z = gcode->get_value('Z');
			if (gcode->has_letter('R')) new_fk_position.Roll = gcode->get_value('R');
			if (gcode->has_letter('P')) new_fk_position.Pitch = gcode->get_value('P');
			if (gcode->has_letter('W')) new_fk_position.Yaw = gcode->get_value('W');
			__planner.arm_solution->SetCurrentPositionAs(&new_fk_position);
			__planner.arm_solution->FK_to_IK(&new_fk_position,&new_ik_position);
			_mover->AllActuator_SetPositionAs(&new_ik_position);
			break;
		default:
			break;
	}
	// Logger::Print("RobotBase::RunMcode() point ",99);
		
}




