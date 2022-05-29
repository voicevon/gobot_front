
#pragma once
#include "AGV/sensor_obstacle/obstacle_sensor_base.h"
#include "AGV/sensor_moving_track/track_sensor_base.h"
#include "AGV/mover_driver/single_wheel_driver/single_wheel_h_bridge_pwm_driver.h"
#include "AGV/mover_driver/mover_dual_wheel.h"
#include "MyBoards/board_base.h"

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

        void Init(BoardbaseAgv* board);
        // void LinkObstacleSensor(ObstacleSensorBase* obstacle_sensor){this->obstacleSensor=obstacle_sensor;};
        // void LinkTrackSensor(TrackSensorBase* trackSensor){this->trackSensor=trackSensor;};
        // void LinkMover(MoverBase* mover){this->__mover=mover;};
        void SpinOnce();
        AgvBase::AGV_STATE GetState(){return this->_State;};
        void ToState(AgvBase::AGV_STATE state);
        void SetFollowMainRoad(bool next_branch_is_on_left, bool follow_main_road);

    protected:
        AGV_STATE _State;
        float _AutoTargetSpeed_in_kmph;
        // void _InitBase();
        void Forwarding();

    private:
        float __full_speed = 100;
        uint8_t __fast_in_percent = 100;
        uint8_t __slow_in_percent = 60;
        uint8_t __parking_in_percent = 10;
        float diff_speed;  //left faster is positive.  //TODO:   should be dir?

        TrackSensorBase* trackSensor;
        ObstacleSensorBase *obstacleSensor;
        MoverBase* __mover;
        // BoardbaseAgv* __board;

        bool DoParking();

};