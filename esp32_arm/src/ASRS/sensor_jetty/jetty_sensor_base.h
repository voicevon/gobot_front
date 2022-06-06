#pragma once
#include <stdint.h>
#include <HardwareSerial.h>

class JettySensorBase{
    public:
        enum IR_POSITION{
            UPPER_INNER = 0,
            UPPER_OUTER = 1,
            LOWER_INNER = 2,
            LOWER_OUTER = 3,
        };
        JettySensorBase(){};
        virtual int16_t ReadAlignmentError(){return 123;};
        virtual float ReadDistance(IR_POSITION irPosition);

    private:

};

