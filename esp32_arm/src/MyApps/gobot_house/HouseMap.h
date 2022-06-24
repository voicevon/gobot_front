#pragma once

class Point{
    public:
        int x;
        int y;
};

class HouseMap{
    public:
        Point rooms[8];
        Point doors[8];
        Point gates[8];
        Point neck;
        Point head;
        void Init();

};