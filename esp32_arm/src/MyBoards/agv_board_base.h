#pragma once

#include "AGV/mover_driver/mover_base.h"
#include "AGV/sensor_obstacle/obstacle_sensor_base.h"
#include "AGV/sensor_moving_track/track_sensor_base.h"
#include "board_base.h"


class AgvBoardbase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);
        virtual ObstacleSensorBase* GetObstacleSensor();
        virtual TrackSensorBase* GetTrackSensor();
        virtual MoverBase* GetMover();

    protected:
        // ObstacleSensorBase* _obstacleSensor;
        // TrackSensorBase* _trackSensor;
        // MoverBase* _mover;

    private:
};
