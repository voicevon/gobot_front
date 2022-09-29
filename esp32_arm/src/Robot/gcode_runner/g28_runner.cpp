#include "g28_runner.h"
#include "MyLibs/basic/logger.h"
#include "CNC/gcode/line_segment.h"


bool G28_Runner::IsDone(){
    return false;
}

//Can deal with:  home via single actuator.
//TODO: Solve this:  Can NOT deal with:  CoreXY, It's combined moving.
void G28_Runner::Start(){ 
    Logger::Debug("G28_Runner::LinkGcode()");
    char axis_name = '+';
    if (this->_gcode->has_letter('X')) axis_name = 'X';
    if (this->_gcode->has_letter('Y')) axis_name = 'Y';
    if (this->_gcode->has_letter('Z')) axis_name = 'Z';
    if (this->_gcode->has_letter('A')) axis_name = 'A';
    if (this->_gcode->has_letter('B')) axis_name = 'B';
    if (this->_gcode->has_letter('C')) axis_name = 'C';
    Logger::Print("home_axis", axis_name);
    EnumAxis axis = CncAxis::GetFromName(axis_name);



    // Is there any machine that supports both IK, and FK homing?
    // this->_home_via_inverse_kinematic = false;


	// if (this->_config_base.IsCombinedFk){
		// this->_RunG28_CombinedFk(axis);

	// }else{
		this->__HomeSingleAxis(axis);
	// }
}

void G28_Runner::__HomeSingleAxis(EnumAxis axis){
	Logger::Debug("G28_Runner::__HomeSingleAxis()");
	Logger::Print("axis", axis);
	// this->_homing_axis = axis;
	// HomingConfig* homing = this->_cnc_homer.GetAxisHomer(axis)->GetHomingConfig();
	HomingConfig* homing;  // = ??
	Logger::Print("homing->IsDirectionToMax", homing->IsDirectionToMax);
	// Gcode gcode = Gcode("G1A6.28");
	// this->_gcode_queue->AppendGcodeCommand("G1A6.28");  //Risk of Gcode queue is full?
	// this->_cnc_board->SayHello();
	// this->_cnc_board->EnableMotor(axis, true);
	// // Serial.println("bbbbbbbbbbbbbbbbbbbbbb");
	
	// // this->_config_base.PrintOut("G28_Runner::__HomeSingleAxis()  _config_base");
	// this->_mover_base->PrintOut("G28_Runner::__HomeSingleAxis()  _mover_base" );
	// this->_mover_base->SetActuatorSpeed(axis, homing->Speed);
	// this->_mover_base->SetActuatorAcceleration(axis, homing->Accelleration);
	// // this->_mover_base->SingleActuatorMoveTo(axis, false, homing->DistanceToGo);
	LineSegment line;
	line.axis = axis;
	line.IsAbsTargetPosition = false;
	// line.TargetPosition = homing->DistanceToGo;
	// line.Speed = homing->Speed;

	// this->_mover_base->SingleActuatorMoveTo(&line);   //TOdo:  Put this line to line_queue

	Logger::Debug("G28_Runner::__HomeSingleAxis() is finished." );
}

void G28_Runner::_running_G28(){
	bool debug = false;
	// if(debug){
	// 	Logger::Debug("G28_Runner::_running_G28() is entering...");
	// 	Logger::Print("_homing_axis", this->_homing_axis);
	// 	auto homer = this->_cnc_homer.GetAxisHomer(this->_homing_axis);
	// 	Logger::Print("Got axis_homer",true);
	// 	auto index = homer->GetTrigeredIndex();
	// 	Logger::Print("Got triggered index", index);
	// }

	// int fired_trigger_index =  this->_cnc_homer.GetAxisHomer(this->_homing_axis)->GetTrigeredIndex();
	// if (fired_trigger_index >=0 ){
	// 	// End stop is trigered
	// 	Logger::Info("G28_Runner::_running_G28() ----> Home sensor is triggered." );
	// 	Logger::Print("_homing_axis_name", this->_homing_axis);
	// 	Logger::Print(" fired_trigger_index", fired_trigger_index);

	// 	// this->_mover_base->AllActuatorsStop();
	// 	// this->_SetCurrentPositionAsHome(this->_homing_axis);
	// 	this->_arm_solution->ForceStopMover();
	// 	this->_arm_solution->_SetCurrentPositionAsHome(this->_homing_axis);
	// 	this->State = RobotState::IDLE_OR_ASYNC;
	// }else{
	// 	// Endstop is not trigered
	// 	// Serial.print(".");
	// 	delay(10);
	// }
}
