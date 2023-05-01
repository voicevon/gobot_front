#pragma once
#include "ms5837.h"

class AquaponicBoard{
    public:
        void Init();
        Ms5837* GetMs5837(){return &__ms5837;};

    private:
        Ms5837 __ms5837;

};