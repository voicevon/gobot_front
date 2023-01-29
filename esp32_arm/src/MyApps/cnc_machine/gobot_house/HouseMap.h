#pragma once

// class Point{
//     public:
//         int x;
//         int y;
// };
#include "CNC/coordinate/coordinate_base.h"

class HouseMap{
    public:
        FKPosition_XYZRPW rooms[8];
        FKPosition_XYZRPW doors[8];
        FKPosition_XYZRPW gates[8];
        FKPosition_XYZRPW neck;
        FKPosition_XYZRPW head;
        void Init();

};