#pragma once

#include "SimpleFOC/common/pid.h"
#include "Robot/Actuator/ActuatorBase.h"
#include "track_sensor_base.h"

enum AGV_DIRECTION{
    FORWARD = 1,
    BACKWARD = 2,
};

class AgvBase{
    public:
        virtual void Stop();
        void SetTargetSpeed(float speed){this->_TargetSpeed = speed;};
        PIDController* speed_pid;
        void LinkTrackSensor(TrackSensorBase* trackSensor){this->trackSensor = trackSensor;};
        TrackSensorBase* trackSensor;

    protected:
        float _TargetSpeed = 0.0f;

    private:


};