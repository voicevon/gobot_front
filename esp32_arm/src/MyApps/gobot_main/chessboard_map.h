#pragma once




#include <stdint.h>
#include <string>
// #include <Robot/robot_hardware_base.h>
#include "CNC/solution/cnc_base.h"

class ChessboardCell{
    public:
        ChessboardCell(char row_name, uint8_t col_index){};
        ChessboardCell(uint8_t row_index, uint8_t col_index){};
        ChessboardCell(String go_cell_name){};
        String GetG1Code(){return "";};  //TODO: Can return arduino string or pointer?

    private:
        void __GetXY(FkPosition_XY* to_xy){};

};

class ChessboardMap{
    String GetG1Code(ChessboardCell* cell){return " ";};  //TODO: Can return arduino string or pointer?

};
