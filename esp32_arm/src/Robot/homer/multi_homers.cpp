#include "multi_homers.h"
#include "MyLibs/basic/logger.h"

MultiPositionHomers::MultiPositionHomers(uint8_t triger_count){
    if (triger_count <= 4){
        this->__triger_count = triger_count;
    } else {
        Logger::Error("MultiPositionHomer::MultiPositionHomer() triger_count must be 4 or smaller...");
        this->__triger_count = 0;
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
