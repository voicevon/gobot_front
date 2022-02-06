#pragma once


/*
.                   1,2,3, ..                         17,18,19
.                  ------------------------------------------
.               A  |                                        |
.               B  |                                        |
.
.   Pay attention: THERE IS NO ROW FOR NAME == 'I'

.               R  |                                        |
.               S  |                                        |
.               T  ------------------------------------------
.
.                                  (W) 
.  "W" is a char code of the special position where connect to stone warehouse            
*/

#include <stdint.h>
#include <string>
#include <Robot/RobotBase.h>

class ChessboardCell{
    public:
        ChessboardCell(char row_name, uint8_t col_index);
        ChessboardCell(String go_cell_name);
        String GetG1Code();  //TODO: Can return arduino string or pointer?

    private:
        void __GetXY(FkPosition_XY* to_xy);

};

class ChessboardMap{
    String GetG1Code(ChessboardCell* cell);  //TODO: Can return arduino string or pointer?

};
