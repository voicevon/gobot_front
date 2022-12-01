#include "g28_runner.h"
#include "MyLibs/basic/logger.h"
#include "CNC/planner/queue_line_segment.h"



bool G28_Runner::IsDone(){
    // Logger::Debug("G28_Runner::IsDone()");
    static unsigned long last_micros;
    PositionTrigger* trigger;
    for(int i=0; i<HomeTrigger_Array::Instance().GetItemsCount(); i++){
        trigger = HomeTrigger_Array::Instance().GetPositionTrigger(i);
        if (trigger->__short_name == this->_axis_name){
            if(trigger->IsFired()){
                Logger::Info("G28_Runner::IsDone()  homer is triggered...");
            	_mover->AllActuatorsStop();
                this->SetHomedPosition(trigger);
	            return true;
            }
        }
    }
    if(micros() - last_micros > 200000){
        Serial.print(".");  //print too fast?
        last_micros = micros();
    }
    return false;
}

void G28_Runner::LinkGcode(Gcode* gcode){
    Logger::Debug("G28_Runner::LinkGcode()");
    Serial.println(gcode->get_command());

    char axis_name = '+';
    // home_actuator_directly = false;
    if (gcode->has_letter('X')) axis_name = 'X';
    if (gcode->has_letter('Y')) axis_name = 'Y';
    if (gcode->has_letter('Z')) axis_name = 'Z';
    if (gcode->has_letter('A')) axis_name = 'A';
    if (gcode->has_letter('B')) axis_name = 'B';
    if (gcode->has_letter('C')) axis_name = 'C';
    if (gcode->has_letter('a')) axis_name = 'a'; 
    if (gcode->has_letter('b')) axis_name = 'b'; 
    if (gcode->has_letter('g')) axis_name = 'g'; 
    if (gcode->has_letter('d')) axis_name = 'd'; 
    if (gcode->has_letter('k')) axis_name = 'k'; 
    if (gcode->has_letter('l')) axis_name = 'l'; 
    Serial.print(char(axis_name));
    Logger::Print("\t\thome_axis", char(axis_name));
    this->_axis_name = axis_name;
}

// Put a move_block to the queue.
// This move_block will set all the involved actuators to move to home direction.
void G28_Runner::Start(){ 
    Logger::Debug("G28_Runner::Start()");

	//Put a move_block into the queue.  Mover will let the actuator to turn...
	MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
    // Logger::Print("G28_Runner::Start() point", 11);
    this->SetMoveBlock_ToHome(_axis_name, mb);
	Queue_MoveBlock::Instance().Deposit();
    // Logger::Print("G28_Runner::Start() point", 4);
	this->_mover->SpinOnce();
    // Logger::Print("G28_Runner::Start() point", 99);

}

