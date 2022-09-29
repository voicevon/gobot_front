#include "g28_runner.h"
#include "MyLibs/basic/logger.h"
#include "CNC/planner/queue_line_segment.h"



bool G28_Runner::IsDone(){
    // Check homer trigger
	// if(homer.is_triggerd()){
	// 	// Stop movement
	// 	mover.force_stop();
	// 	return true;
	// }
	return false;
}

// Put a move_block to the queue.
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

