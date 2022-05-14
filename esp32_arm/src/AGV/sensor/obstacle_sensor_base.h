#pragma once

class ObstacleSensorBase{
    public:
        virtual bool DetectObstacle();
        void SetSafeDistanceToObstacle(int the_distance_in_mm){this->safe_distance_to_obstacle_in_mm=the_distance_in_mm;};
    protected:
        float _distance_to_obstacle;
        int safe_distance_to_obstacle_in_mm;

    private:


};
