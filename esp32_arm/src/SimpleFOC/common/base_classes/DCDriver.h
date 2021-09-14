#ifndef DCDRIVER_H
#define DCDRIVER_H

#include "Arduino.h"

class DCDriver{
    public:
        
        /** Initialise hardware */
        virtual int init() = 0;
};

#endif