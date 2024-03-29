#pragma once
#include "stdint.h"
#include "WString.h"
#include "Wire.h"
#define SENSOR_HISTORY_QUEUE_SIZE  5


class TouchSensor{
    public:
        enum EnumState{
            NOT_INSTALLED,
            CHANNEL_DIED,
            TOUCHED_ON,
            TOUCHED_OFF,
        };
        void Init(uint8_t node_index, uint8_t sensor_index, EnumState state);
        void Review_Sensor_Value(uint8_t new_value);  
        uint8_t GetSensorValue(){return __newest_sensor_value;};
        char GetState();
        void Mute(int mute_ms){__mute_ms_started = millis();};

    private:
        // uint8_t __sensor_id;
        EnumState __state;

        uint8_t __newest_sensor_value;
        uint8_t __untouched_history_values[SENSOR_HISTORY_QUEUE_SIZE];  // TODO: be a queue.
        void __Push_to_HistoryValueWindow(uint8_t new_value);  // newest value at index==0
        uint8_t __node_index;
        uint8_t __sensor_index;
        int __mute_ms_started;
        

};