#include "memory_helper.h"

int MemoryHelper::CompareSame(const void* a, const void* b, int length){
    uint8_t* aa = (uint8_t*) a;
    uint8_t* bb = (uint8_t*) b;
    for(int i=0; i<length ; i++){
        if (*(aa+i) != *(bb+i)){
            return i;
        }
    }
    return -1;
}

