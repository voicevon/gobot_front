#include "g28_runner.h"
#include "MyLibs/basic/logger.h"
#include "CNC/planner/queue_line_segment.h"



bool G28_Runner::IsDone(){
    // Logger::Debug("G28_Runner::IsDone()");
    static unsigned long last_micros;
    PositionTrigger* trigger;
    for(int i=0; i<HomeTrigger_Diction::Instance().GetItemsCount(); i++){
        trigger = HomeTrigger_Diction::Instance().GetPositionTrigger(i);
        if (trigger->AxisName == this->__axis_name){
            if(trigger->IsTriggered()){
                Logger::Info("G28_Runner::IsDone()  homer is triggered...");
            	__mover->AllActuatorsStop();
                this->__last_homed_position = trigger->GetTriggerPosition();
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
    // __homer = HomerDiction::Instance().GetAxisHomer(__axis_name);
    // if (home_actuator_directly){
    //     EnumAxis_Inverseinematic axis_ik = CncAxis::From_FkName_ToEnum(__axis_name);
    //     this->SetMoveBlock_ToHome(axis_ik, mb);
    //     Logger::Print("G28_Runner::Start() point", 31);
    // }else{
    //     EnumAxis_ForwardKinematic axis_fk = CncAxis::From_Ik_Name_ToEnum(__axis_name);
	//     __homer = this->GetHomer(axis_fk);
	//     this->SetMoveBlock_ToHome(axis_fk, mb);
    //     Logger::Print("G28_Runner::Start() point", 32);
    // }

	Queue_MoveBlock::Instance().Deposit();
    Logger::Print("G28_Runner::Start() point", 4);
	this->__mover->SpinOnce();
    Logger::Print("G28_Runner::Start() point", 99);

}

