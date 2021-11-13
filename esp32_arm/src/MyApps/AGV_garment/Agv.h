#pragma once


enum AGV_DIRECTION{
    FORWARD = 1,
    BACKWARD = 2,
};

class AGV{
    public:
        AGV();
        void Move(AGV_DIRECTION direction, int speed_in_percent);
        
    private:

};