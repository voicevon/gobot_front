# pragma once
#include "stdint.h"
#include "WString.h"


class MemoryHelper{
    public:
        // compare byte to byte.
        // return -1 if all bytes are same.
        // return the position index of the first difference.
        static int Find_MismachLocation(const void* a, const void* b, int length);
        // void CopyArduinoString(String from, String to);
};
