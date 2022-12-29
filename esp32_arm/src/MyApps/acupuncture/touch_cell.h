#pragma once
#include "stdint.h"


class TouchCell{
    public:
        uint8_t Address;
        uint8_t CurrentFlags[4];
        uint8_t LastFlags[4];
        bool HasUpdate();
        bool CompareCurrentAndLast();
        void CopyCurrentToLast();
        const char* GetName(int point_id);
        bool IsBitUpdated(int bit_index);
        const char* GetMqttPayload(int bit_index);
        void PrintOut(const char* title);
        bool auto_offline = true;
        const bool& IsOnI2cBus = _is_online;
        void SetOffline(){this->_is_online = false;};
        void SetOnline(){this->_is_online= true;};
        
    private:
        bool _has_update;
        bool _is_online = true;
        char __mqtt_topic_substring[30];


};