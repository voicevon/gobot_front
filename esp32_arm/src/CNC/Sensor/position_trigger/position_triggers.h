#pragma once

#include "CNC/Sensor/position_trigger/single_position_trigger.h"

#define POSITION_TRIGGER_MAX_COUNT 4 
class HomingConfig{
    public:
        bool IsDirectionToMax=true;
        float Speed;
        float Accelleration;
        float DistanceToGo;
        float GetVelocity(){return Speed;};
        bool IsIkPosition=true;
};

class PositionTriggers{
    public:

        PositionTriggers(uint8_t triger_count, HomingConfig* homing_para);
        void AppendSingleHomer(SinglePositionTriger* single_homer);

        //return -1, if no homer is trigered.
        int GetTrigeredIndex();
        HomingConfig* GetHomingConfig(){return this->__homing_config;};
        float GetFiredPosition();

    private:
        uint8_t __triger_count;
        uint8_t __appending_index;
        uint8_t __last_fired_index;

        SinglePositionTriger* _all_single_homers[POSITION_TRIGGER_MAX_COUNT];
        HomingConfig* __homing_config;

};
