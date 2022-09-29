#include "g28_runner.h"
#include "MyLibs/basic/logger.h"
#include "CNC/planner/queue_line_segment.h"



bool G28_Runner::IsDone(){
	if(__homer->GetTrigeredIndex()==-1){
		return false;
	}
	__mover->AllActuatorsStop();
	return true;
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

	//Put a move_block into the queue.  Mover will let the actuator to turn...

	MoveBlock* mb = Queue_MoveBlock::Instance().GetHeadMoveblock();
	this->SetMoveBlock_ToHome(axis, mb);
	Queue_MoveBlock::Instance().ForwardHead();
	this->__mover->SpinOnce();

}

