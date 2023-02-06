#pragma once
#include "MyLibs/board/board_base.h"

#include "AGV/mover_driver/agv_mover_base.h"
#include "AGV/track_light/light_base.h"
#include "AGV/sensor_obstacle/obstacle_sensor_base.h"
#include "AGV/sensor_moving_track/track_sensor_base.h"
#include "AGV/sensor_map_site/map_site_reader.h"


class AgvBoardbase: public BoardBase{
    public:
        virtual void Init();
        virtual ObstacleSensorBase* GetObstacleSensor();
        virtual TrackSensorBase* GetTrackSensor();
        virtual LightBase* GetTrackLight();
        virtual AgvMoverBase* GetMover();
        virtual MapSiteReaderBase* GetMapSiteReader();
        
    protected:

    private:
};
