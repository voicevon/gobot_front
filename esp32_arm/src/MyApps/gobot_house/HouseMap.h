#pragma once

// class Point{
//     public:
//         int x;
//         int y;
// };
#include "CNC/coordinate/coordinate_base.h"

class HouseMap{
    public:
        FkPosition_XY rooms[8];
        FkPosition_XY doors[8];
        FkPosition_XY gates[8];
        FkPosition_XY neck;
        FkPosition_XY head;
        void Init();

};