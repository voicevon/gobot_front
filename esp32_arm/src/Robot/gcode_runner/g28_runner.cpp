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
    Logger::Debug("G28_Runner::Start()");
    char axis_name = '+';
    bool home_actuator_directly = false;
    if (this->_gcode->has_letter('X')) axis_name = 'X';
    if (this->_gcode->has_letter('Y')) axis_name = 'Y';
    if (this->_gcode->has_letter('Z')) axis_name = 'Z';
    if (this->_gcode->has_letter('A')) axis_name = 'A';
    if (this->_gcode->has_letter('B')) axis_name = 'B';
    if (this->_gcode->has_letter('C')) axis_name = 'C';
    if (this->_gcode->has_letter('a')) {axis_name = 'a'; home_actuator_directly=true;}
    if (this->_gcode->has_letter('b')) {axis_name = 'b'; home_actuator_directly=true;}
    if (this->_gcode->has_letter('g')) {axis_name = 'g'; home_actuator_directly=true;}
    if (this->_gcode->has_letter('d')) {axis_name = 'd'; home_actuator_directly=true;}
    Logger::Print("home_axis", axis_name);
    Logger::Print("G28_Runner::Start() point", 1);

	//Put a move_block into the queue.  Mover will let the actuator to turn...
	MoveBlock* mb = Queue_MoveBlock::Instance().GetHeadMoveblock();
    Logger::Print("G28_Runner::Start() point", 2);
    if (home_actuator_directly){
        EnumAxis_Inverseinematic axis_ik = CncAxis::InverserKinematic_Axis(axis_name);
        this->SetMoveBlock_ToHome(axis_ik, mb);
    }else{
        EnumAxis_ForwardKinematic axis_fk = CncAxis::ForwardKinematic_Axis(axis_name);
	    this->SetMoveBlock_ToHome(axis_fk, mb);
        Logger::Print("G28_Runner::Start() point", 3);
    }

	Queue_MoveBlock::Instance().ForwardHead();
    Logger::Print("G28_Runner::Start() point", 4);
	this->__mover->SpinOnce();
    Logger::Print("G28_Runner::Start() point", 99);

}

