#pragma once

#include "SimpleFOC/common/pid.h"
#include "Robot/Actuator/ActuatorBase.h"
#include "track_sensor_base.h"



class VehicalBase{
        public:
            enum VEHICAL_DIRECTION{
                FORWARD = 1,
                BACKWARD = 2,
            };
        virtual void Init();
        virtual void Stop();
        virtual void MoveForward(int track_error);
        void SetTargetSpeed(int speed){this->_TargetSpeed = speed;};
        PIDController* speed_pid;
        // void LinkTrackSensor(TrackSensorBase* trackSensor){this->trackSensor = trackSensor;};
        // TrackSensorBase* trackSensor;

    protected:
        int _TargetSpeed =0;
        int _LastError;

    private:


};