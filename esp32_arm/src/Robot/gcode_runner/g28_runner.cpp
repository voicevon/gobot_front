#include "g28_runner.h"
#include "MyLibs/basic/logger.h"
#include "CNC/planner/line_segments_queue.h"



bool G28_Runner::IsDone(){
    // Logger::Debug("G28_Runner::IsDone()");
    if (_has_position_trigger){
        static unsigned long last_micros;
        PositionTrigger* trigger;
        for(int i=0; i<gs_PositionTrigger_Array::Instance().GetItemsCount(); i++){
            trigger = gs_PositionTrigger_Array::Instance().GetPositionTrigger(i);
            if (trigger->MyName == this->_axis_name){
                if(trigger->IsFired()){
                    Logger::Info("G28_Runner::IsDone()  homer is triggered...");
                    _mover->AllActuatorsStop();
                    this->_SetHomedPosition(trigger);
                    return true;
                }
            }
        }
        if(micros() - last_micros > 200000){
            Serial.print(".");  //print too fast?
            last_micros = micros();
        }
        return false;
    }else{
        return true;
    }
}

void G28_Runner::LinkGcode(GcodeText* gcode_text){
    Logger::Debug("G28_Runner::LinkGcode()");
    
    GcodeHelper gcode_helper(gcode_text->bytes);
    GcodeHelper* gcode = &gcode_helper;
    Serial.println(gcode->get_command());
    char axis_name = '+';
    // home_actuator_directly = false;
    if (gcode->has_letter('X')) axis_name = 'X';
    if (gcode->has_letter('Y')) axis_name = 'Y';
    if (gcode->has_letter('Z')) axis_name = 'Z';
    if (gcode->has_letter('R')) axis_name = 'R';
    if (gcode->has_letter('P')) axis_name = 'P';
    if (gcode->has_letter('W')) axis_name = 'W';
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
	MoveBlock* mb = gs_MoveBlock_Queue::Instance().GetRoom_ForDeposit();
    // Logger::Print("G28_Runner::Start() point", 11);
    // mb->PrintOut("Test mb is none or not.  Twh_LoopPorter_G28_Runner::_SetMoveBlock_ToHome()");
    this->_SetMoveBlock_ToHome(_axis_name, mb);
	gs_MoveBlock_Queue::Instance().Deposit();
    // Logger::Print("G28_Runner::Start() point", 4);
	this->_mover->SpinOnce();
    // Logger::Print("G28_Runner::Start() point", 99);

}

