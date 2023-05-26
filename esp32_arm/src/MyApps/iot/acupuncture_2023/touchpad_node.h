#pragma once

#include "MyLibs/board/peripheral/bus_i2c/i2c_slave_node_agent.h"
#include "touchpad_sensor.h"
#include "WString.h"


#define TOUCH_PAD_SENSORS_COUNT_IN_NODE 14

class TouchPad_Node: public I2C_SlaveNodeAgent{
    public:
        void Init( uint8_t i2c_slave_address, bool is_installed);
        void Process_RxBuffer();

        char GetState();
        // return -1, if nothing is updated.
        int GetUpdatedSensor();
        String GetSensorsState();
        TouchSensor* GetSingleSensor(int sensor_index){return &__all_sensors[sensor_index];};
        
    private:
        TouchSensor __all_sensors[TOUCH_PAD_SENSORS_COUNT_IN_NODE];
        uint8_t __rx_buffer[TOUCH_PAD_SENSORS_COUNT_IN_NODE];
        char __previous_sensros_state[14];

};