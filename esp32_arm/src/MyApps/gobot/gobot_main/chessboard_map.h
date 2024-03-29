#pragma once

#include <stdint.h>
#include <WString.h>
#include "CNC/coordinate/coordinate_base.h"


class ChessboardCell{
    public:
        ChessboardCell(char row_name, uint8_t col_index){};
        ChessboardCell(uint8_t row_index, uint8_t col_index){};
        ChessboardCell(String gogame_cell_name){};
        String GetG1Code(){return "";};  //TODO: Can return arduino string or pointer?

    private:
        void __GetXY(FKPosition_XYZRPW* to_xy){};

};

class ChessboardMap{
    String GetG1Code(ChessboardCell* cell);

};
