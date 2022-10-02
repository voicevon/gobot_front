#pragma once

#include "position_trigger.h"

#define POSITION_TRIGGER_MAX_COUNT 4 

// TODO:  remove this class
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

#include "MyLibs/array/array_base.h"
// #include "MyLibs/dictions/diction_base.h"
class AxisHomer: public ArrayItem{
    public:
        AxisHomer(){};
        AxisHomer(uint8_t triger_count);
        void AppendPositionTrigger(PositionTrigger* single_homer);

        //return -1, if no homer is trigered.
        int GetTrigeredIndex();
        float GetFiredPosition();
        HomingConfig* GetHomingConfig(){return &this->__homing_config;};

    private:
        uint8_t __appended_count = 0;
        uint8_t __last_fired_index = -1;

        PositionTrigger* __triggers_diction[POSITION_TRIGGER_MAX_COUNT];
        HomingConfig __homing_config;

};
