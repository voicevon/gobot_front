#pragma once


#include <Wire.h>
#include "i2c_slave_node.h"


//  Pull-up resistor = 1.8K   to 5V

class I2C_Master{
    public:
        void Init();
        bool ReadSlaveNode(I2C_SlaveNode* slave_node);

    private:
};