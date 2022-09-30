#include "g28_runner.h"
#include "MyLibs/basic/logger.h"
#include "CNC/planner/queue_line_segment.h"



bool G28_Runner::IsDone(){
    Logger::Debug("G28_Runner::IsDone()");
	if(__homer->GetTrigeredIndex()==-1){
        Serial.print(".");  //print too fast?
		return false;
	}
    Logger::Info("G28_Runner::IsDone()  homer is triggered...");
    

	__mover->AllActuatorsStop();
	return true;
}

void G28_Runner::LinkGcode(Gcode* gcode){
    Logger::Debug("G28_Runner::LinkGcode()");
    Serial.println(gcode->get_command());

    char axis_name = '+';
    home_actuator_directly = false;
    if (gcode->has_letter('X')) axis_name = 'X';
    if (gcode->has_letter('Y')) axis_name = 'Y';
    if (gcode->has_letter('Z')) axis_name = 'Z';
    if (gcode->has_letter('A')) axis_name = 'A';
    if (gcode->has_letter('B')) axis_name = 'B';
    if (gcode->has_letter('C')) axis_name = 'C';
    if (gcode->has_letter('a')) {axis_name = 'a'; home_actuator_directly=true;}
    if (gcode->has_letter('b')) {axis_name = 'b'; home_actuator_directly=true;}
    if (gcode->has_letter('g')) {axis_name = 'g'; home_actuator_directly=true;}
    if (gcode->has_letter('d')) {axis_name = 'd'; home_actuator_directly=true;}
    Serial.print(char(axis_name));
    Logger::Print("\t\thome_axis", char(axis_name));
    this->__axis_name = axis_name;
}

// Put a move_block to the queue.
void G28_Runner::Start(){ 
    Logger::Debug("G28_Runner::Start()");

    Logger::Print("G28_Runner::Start() point", 1);

	//Put a move_block into the queue.  Mover will let the actuator to turn...
	MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
    // Logger::Print("G28_Runner::Start() point", 11);
    // mb->DeepReset_ToDefault();
    // Logger::Print("G28_Runner::Start() point", 12);
    // MoveBlock_SingleActuator* aa = &mb->MoveBlocks[0];
    // Logger::Print("G28_Runner::Start() point", 13);
    // aa->TargetPosition = 12;
    // Logger::Print("G28_Runner::Start() point", 14);

    Logger::Print("G28_Runner::Start() point", 2);
    if (home_actuator_directly){
        EnumAxis_Inverseinematic axis_ik = CncAxis::InverserKinematic_Axis(__axis_name);
        this->SetMoveBlock_ToHome(axis_ik, mb);
        Logger::Print("G28_Runner::Start() point", 31);
    }else{
        EnumAxis_ForwardKinematic axis_fk = CncAxis::ForwardKinematic_Axis(__axis_name);
	    this->SetMoveBlock_ToHome(axis_fk, mb);
        Logger::Print("G28_Runner::Start() point", 32);
    }

	Queue_MoveBlock::Instance().Deposit();
    Logger::Print("G28_Runner::Start() point", 4);
	this->__mover->SpinOnce();
    Logger::Print("G28_Runner::Start() point", 99);

}

