#include "robot_base.h"
#include "MyLibs/MyFunctions.hpp"
#include "HardwareSerial.h"
#include "MyLibs/message_queue.h"
#include "CNC/arm_solution/arm_solution_base.h"


void RobotBase::SpinOnce(){
	// Logger::Debug("RobotBase::SpinOnce()");
	this->_mover->SpinOnce();
	switch (this->State){
		case RobotState::G4_IS_SYNCING:
			//todo:  when buffer is empty,  a gcode is still runnning.
			// Correct way is:  buffer is empey, and Mover is stoped.
			if (this->__queue_move_block.BufferIsEmpty()){
				this->__g4_runner.Start();
				this->State = RobotState::G4_IS_RUNNING;
			}
			break;
		case RobotState::G4_IS_RUNNING:
			if(this->__g4_runner.IsDone()){
				this->State =RobotState::IDLE_OR_ASYNC;
			}
			break;
		case RobotState::G28_IS_SYNCING:
			if (this->__queue_move_block.BufferIsEmpty()){
				this->__g28_runner.Start();
				this->State = RobotState::G28_IS_RUNNING;
			}
			break;
		case RobotState::G28_IS_RUNNING:
			if(this->__g28_runner.IsDone()){
				this->State = RobotState::IDLE_OR_ASYNC;
			}
			break;

		default:
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
	Logger::Print("RobotBase::SpinOnce() point", 99);

}

void RobotBase::__RunGcode(Gcode* gcode){
	char home_axis_name = '+';
	LineSegment line;
	EnumAxis home_axis;

	switch (gcode->g){
		case 28:
			this->State = RobotState::G28_IS_SYNCING;
			if (gcode->has_letter('X')) home_axis_name='X';
			if (gcode->has_letter('Y')) home_axis_name='Y';
			if (gcode->has_letter('Z')) home_axis_name='Z';
			if (gcode->has_letter('A')) home_axis_name='A';
			if (gcode->has_letter('B')) home_axis_name='B';
			if (gcode->has_letter('C')) home_axis_name='C';
			if (gcode->has_letter('W')) home_axis_name='W';
			Logger::Debug("RobotBase::RunGcode()    G28");
			Logger::Print("home_axis",home_axis_name);

			// Is there any machine that supports both IK, and FK homing?
			// this->_home_via_inverse_kinematic = false;
			if (home_axis_name == '+'){
				Serial.print("\n\n\n\n[Error] RobotBase::RunGcode()  :");
				Serial.print(home_axis_name);

			}
			//TODO:  convert char to enum
			// this->RunG28(this->ConvertToEnum(home_axis));
			home_axis =  this->_arm_solution->ConvertToEnum(home_axis_name);
			this->__g28_runner.HomingAxis = home_axis;
			this->State = RobotState::G28_IS_SYNCING;
			// this->RunG28(home_axis);
			// this->commuDevice->OutputMessage(COMMU_OK);  For calble-bot-corner, it should be 'Unknown Command'
			break;
		case 4:
			// G4 Dwell, Pause for a period of time.
			this->__g4_runner.SetTarget_in_ms(gcode->get_value('S'));
			this->State = RobotState::G4_IS_SYNCING;
			break;
						
		case 1:
			// G1 Move
			//TODO:  1. put position to movement queue. called "plan" in smoothieware? 
			//       2. send out OK.
			//       3. Set status to busy.
			//       4. Start Moving.
			this->__planner.__arm_solution->_ConvertG1ToLineSegment(gcode, &line);
			this->__planner.AppendLineSegment(&line);

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
	Logger::Print("RobotBase::RunMcode() point ",99);
		
}

void RobotBase::__RunMcode(Gcode* gcode){
	bool debug = false;
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
			Logger::Debug("RobotBase::RunGcode()   M130");
			Logger::Print("gcode", gcode->get_command());
			p_value =  gcode->get_value('N');
			Logger::Print("Index", p_value);
			
			f_value = gcode->get_value('P');
			this->__pid_controllers_m130->GetController(p_value)->P = f_value;
			Logger::Print("P", f_value);

			f_value = gcode->get_value('I');
			this->__pid_controllers_m130->GetController(p_value)->I = f_value;
			Logger::Print("I", f_value);

			f_value = gcode->get_value('D');
			this->__pid_controllers_m130->GetController(p_value)->D = f_value;
			Logger::Print("D", f_value);

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
	Logger::Print("RobotBase::RunMcode() point ",99);

}

//Can deal with:  home via single actuator.
//TODO: Solve this:  Can NOT deal with:  CoreXY, It's combined moving.
void RobotBase::RunG28(EnumAxis axis){ 
	bool debug = true;
	if (debug){
		Logger::Debug("RobotBase::RunG28() is entering" );
		Logger::Print("axis", axis);
		Logger::Print("IsCombinedFK", this->_config_base.IsCombinedFk);
	}

	if (this->_config_base.IsCombinedFk){
		// this->_RunG28_CombinedFk(axis);

	}else{
		this->__HomeSingleAxis(axis);
	}
}

void RobotBase::__HomeSingleAxis(EnumAxis axis){
	Logger::Debug("RobotBase::__HomeSingleAxis()");
	Logger::Print("axis",axis);
	this->_homing_axis = axis;
	HomingConfig* homing = this->_cnc_homer.GetAxisHomer(axis)->GetHomingConfig();
	Logger::Print("homing->IsDirectionToMax", homing->IsDirectionToMax);
	// Gcode gcode = Gcode("G1A6.28");
	this->_gcode_queue->AppendGcodeCommand("G1A6.28");  //Risk of Gcode queue is full?
	// this->_cnc_board->SayHello();
	// this->_cnc_board->EnableMotor(axis, true);
	// // Serial.println("bbbbbbbbbbbbbbbbbbbbbb");
	
	// // this->_config_base.PrintOut("RobotBase::__HomeSingleAxis()  _config_base");
	// this->_mover_base->PrintOut("RobotBase::__HomeSingleAxis()  _mover_base" );
	// this->_mover_base->SetActuatorSpeed(axis, homing->Speed);
	// this->_mover_base->SetActuatorAcceleration(axis, homing->Accelleration);
	// // this->_mover_base->SingleActuatorMoveTo(axis, false, homing->DistanceToGo);
	// LineSegment line;
	// line.axis = axis;
	// line.IsAbsTargetPosition = false;
	// line.TargetPosition = homing->DistanceToGo;
	// line.Speed = homing->Speed;
	// this->_mover_base->SingleActuatorMoveTo(&line);   //TOdo:  Put this line to line_queue

	Logger::Debug("RobotBase::__HomeSingleAxis() is finished." );
}

void RobotBase::_running_G28(){
	bool debug = false;
	if(debug){
		Logger::Debug("RobotBase::_running_G28() is entering...");
		Logger::Print("_homing_axis", this->_homing_axis);
		auto homer = this->_cnc_homer.GetAxisHomer(this->_homing_axis);
		Logger::Print("Got axis_homer",true);
		auto index = homer->GetTrigeredIndex();
		Logger::Print("Got triggered index", index);
	}

	int fired_trigger_index =  this->_cnc_homer.GetAxisHomer(this->_homing_axis)->GetTrigeredIndex();
	if (fired_trigger_index >=0 ){
		// End stop is trigered
		Logger::Info("RobotBase::_running_G28() ----> Home sensor is triggered." );
		Logger::Print("_homing_axis_name", this->_homing_axis);
		Logger::Print(" fired_trigger_index", fired_trigger_index);

		// this->_mover_base->AllActuatorsStop();
		// this->_SetCurrentPositionAsHome(this->_homing_axis);
		this->_arm_solution->ForceStopMover();
		this->_arm_solution->_SetCurrentPositionAsHome(this->_homing_axis);
		this->State = RobotState::IDLE_OR_ASYNC;
	}else{
		// Endstop is not trigered
		// Serial.print(".");
		delay(10);
	}
}

// void RobotBase::RunM123(uint8_t eef_channel, uint8_t eef_action){
// 	Logger::Debug("RobotBase::RunM123()");
// 	Logger::Print("eef_action", eef_action);
// 	// uint8_t action_code = 1;
// 	this->__eef->PrintOut();
// 	this->__eef->Run(eef_action);
// }

void RobotBase::RunM84(){
	//TODO: CNC_AXIS_COUNT_IK,   vs CNC_AXIS_COUNT_FK
	for (int axis=0; axis<CNC_AXIS_COUNT; axis++){
		this->_cnc_board->EnableMotor(EnumAxis(axis), false);
	}
}

void RobotBase::Run_M42_OutputGpio(uint8_t pin_number, uint8_t pin_value){
	digitalWrite(pin_number, pin_value);
}
