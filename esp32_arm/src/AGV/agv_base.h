/*
.   Objects:
.       BoxMover
.       TwinWheels
*/





#pragma once
#include "AGV/sensor_obstacle/obstacle_sensor_base.h"
#include "AGV/sensor_moving_track/track_sensor_base.h"
#include "AGV/mover_driver/single_wheel_driver/single_wheel_h_bridge_pwm_driver.h"
#include "AGV/mover_driver/mover_dual_wheel.h"


class AgvBase{
    public:
        enum AGV_STATE{
            FAST_MOVING = 1,
            FAST_MOVING_PAUSED = 2,
            SLOW_MOVING = 3,
            SLOW_MOVING_PAUSED = 4,
            PARKING = 5,
            PARKING_PAUSED = 6,
            PARKED = 7,
        };

        void Init(){this->ToState(SLOW_MOVING_PAUSED);};
        void LinkObstacleSensor(ObstacleSensorBase* obstacle_sensor){this->obstacleSensor=obstacle_sensor;};
        void LinkTrackSensor(TrackSensorBase* trackSensor){this->trackSensor=trackSensor;};
        void LinkMover(MoverBase* mover){this->__mover=mover;};
        void SpinOnce();
        AgvBase::AGV_STATE GetState(){return this->_State;};
        void ToState(AgvBase::AGV_STATE state);
        void SetFollowMainRoad(bool next_branch_is_on_left, bool follow_main_road);

    protected:
        void _InitBase();
        AGV_STATE _State;
        void Forwarding();
        float common_speed;

    private:
        float __fast_velocity = 100;
        float __slow_velocity = 60;
        float __parking_velocity = 10;
        float diff_speed;  //left faster is positive.  //TODO:   should be dir?

        TrackSensorBase* trackSensor;
        ObstacleSensorBase *obstacleSensor;
        MoverBase* __mover;

        bool DoParking();

};