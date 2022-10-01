#include "Robot/axis_homer/axis_homer.h"
#include "MyLibs/basic/logger.h"

AxisHomer::AxisHomer(uint8_t trigger_count){
    this->__appended_count = 0;
    this->__last_fired_index = -1;
    // this->__is_home_direction_to_max = is_home_direction_to_max;
    // this->__max_distance_to_home = max_distance_to_home;
    // this->__homing_config = config;
    // this->__appending_index = 0;
    // if (trigger_count <= 4){
    //     this->__trigger_count = trigger_count;
    // } else {
    //     Logger::Error("MultiPositionHomer::MultiPositionHomer() triger_count must be 4 or smaller...");
    // }
}

void AxisHomer::AppendPositionTrigger(PositionTrigger* single_homer){
    if (this->__appended_count < 4){
        this->__triggers_diction[this->__appended_count] = single_homer;
        Logger::Info("AxisHomer::AppendPositionTrigger()");
        Logger::Print("Total appened count", this->__appended_count);
        this->__appended_count++;

    } else {
        Logger::Warn("AxisHomer::AppendPositionTrigger()   TOO MANY HAS BEEN APPENED! ");
    }
}

int AxisHomer::GetTrigeredIndex(){
    // Logger::Debug("AxisHomer::GetTrigeredIndex() is entering...");
    // Logger::Print("__appended_count", __appended_count);
    for (uint8_t i=0; i<this->__appended_count; i++){
        // Logger::Print("inside for loop ", i);
        // auto aa = this->__triggers_diction[i];
        // Logger::Print("Got this trigger, index", i);
        if (this->__triggers_diction[i]->IsTriggered()){
            Logger::Print("This trigger is fired", i);
            this->__last_fired_index = i;    
            return i;
        }
    }
    // There is no trigering.
    return -1;
}

float AxisHomer::GetFiredPosition(){
    return this->__triggers_diction[this->__last_fired_index]->GetTriggerPosition();
}
