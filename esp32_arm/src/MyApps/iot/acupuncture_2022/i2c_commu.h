#pragma once

#include <Wire.h>
#include "touch_cell.h"


//  Pull-up resistor = 1.8K   to 5V

class I2c_commu{
    public:
        void Init(int min_cell_i2c_address, int cells_count);
        TouchCell* FindandReadValidateCell();   // return the last slave_id of avaliable.
        void SpinOnce();
        bool ReadSingleCell(TouchCell* cell);
        bool HasUpdate();
        TouchCell Cells[30];

    private:
        int __CELLS_COUNT;
        // int __CELL_I2C_ADDRESS_MIN;
        bool _has_update = true;
};