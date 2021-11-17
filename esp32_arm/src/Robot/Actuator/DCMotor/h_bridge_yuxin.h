#pragma once
#include "DCDriverHBridge.h"



class YuXin_HBridge:public DCDriverHBridge{

    public:
        YuXin_HBridge(){};
        void Init(uint8_t pin_a,  uint8_t pin_b);
        void Start(int pwm_speed, bool dir_forward) override;
        void Stop() override;
        void Break() override;

    protected:
        // uint8_t _pin_enable;

};
