#include "memory_helper.h"

int MemoryHelper::Find_MismachLocation(const void* a, const void* b, int length){
    uint8_t* aa = (uint8_t*) a;
    uint8_t* bb = (uint8_t*) b;
    for(int i=0; i<length ; i++){
        if (*(aa+i) != *(bb+i)){
            return i;
        }
    }
    return -1;
}

String MemoryHelper::humanReadableSize(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}