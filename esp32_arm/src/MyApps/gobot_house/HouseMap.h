#pragma once

// class Point{
//     public:
//         int x;
//         int y;
// };
#include "CNC/coordinate/coordinate_base.h"

class HouseMap{
    public:
        FKPosition_XYZRPY rooms[8];
        FKPosition_XYZRPY doors[8];
        FKPosition_XYZRPY gates[8];
        FKPosition_XYZRPY neck;
        FKPosition_XYZRPY head;
        void Init();

};