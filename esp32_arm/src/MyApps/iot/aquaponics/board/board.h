#pragma once
#include "ms5837.h"
#include <Wire.h>
class AquaponicBoard{
    public:
        void Init();
        Ms5837* GetMs5837(){return &__ms5837;};

    private:
        Ms5837 __ms5837;
        TwoWire __my_i2c = TwoWire(0);
};