#pragma once

#include "single_position_trigger.h"

#define POSITION_TRIGGER_MAX_COUNT 4 


class HomingConfig{
    public:
        bool IsDirectionToMax=true;
        float Speed;
        float Accelleration;
        float DistanceToGo;
        float GetVelocity(){return Speed;};
        bool IsIkPosition = true;
        float LastHomedPosition = 0;
};

class AxisHomer{
    public:

        AxisHomer(uint8_t triger_count, HomingConfig* homing_para);
        void AppendPositionTrigger(SinglePositionTrigger* single_homer);

        //return -1, if no homer is trigered.
        int GetTrigeredIndex();
        float GetFiredPosition();
        HomingConfig* GetHomingConfig(){return this->__homing_config;};

    private:
        uint8_t __trigger_count;
        uint8_t __appending_index;
        uint8_t __last_fired_index;

        SinglePositionTrigger* _all_single_homers[POSITION_TRIGGER_MAX_COUNT];
        HomingConfig* __homing_config;

};
