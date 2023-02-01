#pragma once


#include <Wire.h>
#include "touch_cell.h"




//  Pull-up resistor = 1.8K   to 5V

class I2c_commu_2023{
    public:
        void Init(int min_cell_i2c_address, int cells_count);
        TouchCell_2023* FindandReadValidateCell();   // return the last slave_id of avaliable.
        void SpinOnce();
        bool ReadSingleCell(TouchCell_2023* cell);
        bool HasUpdate();
        TouchCell_2023 Cells[30];

    private:
        int __CELLS_COUNT;
        // int __CELL_I2C_ADDRESS_MIN;
        bool _has_update = true;
};