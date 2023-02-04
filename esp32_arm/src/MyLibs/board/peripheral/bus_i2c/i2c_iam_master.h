#pragma once


#include <Wire.h>
#include "i2c_slave_node_agent.h"


//  Pull-up resistor = 1.8K   to 5V

class I2C_IamMaster{
    public:
        void Init();
        bool ReadSlaveNode(I2C_SlaveNodeAgent* slave_node);
        //TODO: bool WriteSlaveNode(node, length);
    private:
};