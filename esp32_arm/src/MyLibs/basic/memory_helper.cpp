#include "memory_helper.h"

bool MemoryHelper::CompareSame(void* a, void* b, int length){
    uint8_t* aa = (uint8_t*) a;
    uint8_t* bb = (uint8_t*) b;
    for(int i=0; i<length ; i++){
        if ((*aa) != (*bb)){
            return false;
        }
    }
    return true;
}