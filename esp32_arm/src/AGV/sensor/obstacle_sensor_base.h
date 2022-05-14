#pragma once

class ObstacleSensorBase{
    public:
        virtual bool DetectObstacle();

    protected:
        float _distance_to_obstacle;
        
    private:


};
