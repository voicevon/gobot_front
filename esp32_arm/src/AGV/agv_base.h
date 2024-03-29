
#pragma once
#include "AGV/sensor_obstacle/obstacle_sensor_base.h"
#include "AGV/sensor_moving_track/track_sensor_base.h"
#include "AGV/mover_driver/mover_dual_wheel.h"
#include "board/agv_board_base.h"
#include "sensor_map_site/smart_rfid_reader.h"

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

        void Init(AgvBoardbase* board);
        void SpinOnce();
        AgvBase::AGV_STATE GetState(){return this->_State;};
        void ToState(AgvBase::AGV_STATE state);
        void SetFollowMainRoad(bool next_branch_is_on_left, bool follow_main_road);
        void LinkRfidReader(SmartRfidReader* rfidReader);
        SmartRfidReader* RfidReader;  // TODO: to be private
        float ReadBattery();

    protected:
        AGV_STATE _State;
        float _AutoTargetSpeed_in_kmph;
        void Forwarding();

    private:
        float __full_speed = 100;
        uint8_t __fast_in_percent = 100;
        uint8_t __slow_in_percent = 60;
        uint8_t __parking_in_percent = 10;
        float diff_speed;  //left faster is positive.  //TODO:   should be dir?

        TrackSensorBase* trackSensor;
        ObstacleSensorBase *obstacleSensor;
        AgvMoverBase* __mover;
        AgvBoardbase* __board;   //TODO,  remove this,  to create ClassBatteryReader.

        bool DoParking();

};