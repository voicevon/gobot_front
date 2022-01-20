#pragma once

#include "AGV/track_sensor_base.h"

class LogicRemoteSensorBase{
    public:
        virtual void FromRawData(uint8_t* raw_data);
};

class PositionSensor: public LogicRemoteSensorBase{
    public:
        int8_t ErrorValue = 0;
        void FromRawData(uint8_t* raw_data) override;
};

class TrackSensor: public LogicRemoteSensorBase, public TrackSensorBase{
    public:
        int8_t ErrorValue = 0;
        void FromRawData(uint8_t* raw_data) override;
        // TrackSensor_Shengteng(){};
        int ReadError_FromRight(uint8_t* rxBuffer) override;
};

class ObstacleSensor: public LogicRemoteSensorBase{
    public:
        bool BLOCKING= false;
        void FromRawData(uint8_t* raw_data) override;
};

class RfidReader: public LogicRemoteSensorBase{
    public:
        uint16_t last_card_id;
        void FromRawData(uint8_t* raw_data) override;
};

class RemoteSensor{
    public:
        PositionSensor ObjPositionSensor;
        TrackSensor ObjTrackSensor;
        ObstacleSensor ObjObstacleSensor;
        RfidReader ObjRfidReader;
        void ReadRawData();
    private:
        uint8_t __raw_data[5];
};



