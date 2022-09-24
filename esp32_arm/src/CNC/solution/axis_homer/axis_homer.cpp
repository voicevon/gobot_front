// #include "CNC/Sensor/position_trigger/position_triggers.h"
#include "CNC/solution/axis_homer/axis_homer.h"
#include "MyLibs/basic/logger.h"

AxisHomer::AxisHomer(uint8_t trigger_count){
    this->__trigger_count = 0;
    // this->__is_home_direction_to_max = is_home_direction_to_max;
    // this->__max_distance_to_home = max_distance_to_home;
    // this->__homing_config = config;
    this->__appending_index = 0;
    if (trigger_count <= 4){
        this->__trigger_count = trigger_count;
    } else {
        Logger::Error("MultiPositionHomer::MultiPositionHomer() triger_count must be 4 or smaller...");
    }
}

void AxisHomer::AppendPositionTrigger(SinglePositionTrigger* single_homer){
    this->_all_single_homers[this->__appending_index] = single_homer;
    this->__appending_index++;
    if (this->__appending_index > this->__trigger_count){
        Logger::Warn("AxisHomer::AppendPositionTrigger()   TOO MANY HAS BEEN APPENED! ");
    }
}

int AxisHomer::GetTrigeredIndex(){
    for (uint8_t i=0; i<this->__trigger_count; i++){
        if (this->_all_single_homers[i]->IsTriggered()){
            this->__last_fired_index = i;    
            return i;
        }
    }
    // There is no trigering.
    return -1;
}

float AxisHomer::GetFiredPosition(){
    return this->_all_single_homers[this->__last_fired_index]->GetTriggerPosition();
}
