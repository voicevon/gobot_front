// #include "multi_homers.h"
#include "Robot/position_trigger/position_triggers.h"
#include "MyLibs/basic/logger.h"

MultiPositionHomers::MultiPositionHomers(uint8_t triger_count){
    this->__triger_count = 0;
    this->__appending_index = 0;
    if (triger_count <= 4){
        this->__triger_count = triger_count;
    } else {
        Logger::Error("MultiPositionHomer::MultiPositionHomer() triger_count must be 4 or smaller...");
    }
}

void MultiPositionHomers::AppendSingleHomer(SinglePositionTriger* single_homer){
    this->_all_single_homers[this->__appending_index] = single_homer;
    this->__appending_index++;
    if (this->__appending_index > this->__triger_count){
        Logger::Warn("MultiPositionHomers::AppendSingleHomer()   TOO MANY HAS BEEN APPENED! ");
    }
}

uint8_t MultiPositionHomers::GetTrigeredIndex(){
    for (uint8_t i=0; i<this->__triger_count; i++){
        if (this->_all_single_homers[i]->IsTriged())
            return i;
    }
    // There is no trigering.
    return -1;
}
