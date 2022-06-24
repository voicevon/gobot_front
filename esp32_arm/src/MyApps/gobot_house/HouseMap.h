#pragma once

class Point{
    public:
        int x;
        int y;
};
#include "CNC/coordinate/coordinate_base.h"

class HouseMap{
    public:
        Point rooms[8];
        Point doors[8];
        Point gates[8];
        Point neck;
        Point head;
        void Init();

};