#pragma once
#include "stdint.h"
#include "touchpad_channel.h"

//TODO: rename to I2c_SlaveAgent
class TouchCell_2023{
    public:
        uint8_t I2C_Address;
        void Init(uint8_t cell_id, uint8_t installed_channel_count);
        // void AppendChannel(TouchPad_Channel* channel);


        uint8_t CurrentFlags[4];
        uint8_t LastFlags[4];
        bool HasUpdate();
        void CopyCurrentToLast();
        const char* GetName(int point_id);
        // bool IsBitUpdated(int bit_index);
        const char* GetMqttPayload(int bit_index);
        bool IsForceOnline = false;
        const bool& IsOnline = _is_online;
        void SetOffline(){this->_is_online = false;};
        void SetOnline(){this->_is_online= true;};

        uint8_t* GetRxBuffer(){return __rx_buffer;};
        bool Review_RxBuffer();
        
    private:
        bool _is_online = true;
        char __mqtt_topic_substring[30];
        TouchPad_Channel __all_channels[14];
        uint8_t __rx_buffer[16];
        bool __has_changed_channel;


};