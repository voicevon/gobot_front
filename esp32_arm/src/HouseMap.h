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
        Point neck;
        Point head;
        float distance_room_to_door[8];
        float distance_head_to_neck;
        void setup(int segments);
        float get_dx_distance(int house_id);
        float get_dy_distance(int house_id);
        // Point get_target_position(int segment_id);  
        int get_segments(){return __segments;};

    private:
        int __segments;
        // float __house_distance[8];
        float __head_distance;
        void __init();
};