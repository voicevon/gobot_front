# pragma once
#include "stdint.h"

class MemoryHelper{
    public:
        // compare byte to byte.
        // return true if all bytes are same.
        // return false if any is different.
        static bool CompareSame(void* a, void* b, int length);
};
