#pragma once

#include "MyLibs/board/board_base.h"
#include "MyLibs/component/led_gpio.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/axis_homer/position_trigger_array.h" 
#include "FastAccelStepper.h"

#define HOME_TRIGGER_COUNT 1

class Twh2Row_Board: public BoardBase{
    public:
        Twh2Row_Board();
        void Init() override;
        
        // HX711* GetHx711(){return &this->__hx711;};
        FastAccelStepper* GetStepper(){return __stepper_alpha;};
        void TurnOn_ThisLed_Only(int led_id);

        void Test_PositionTriggers(int loops);
        void Test_Stepper(int loops);
        void Test_StepperQueue(int loops);

    protected:

    private:

        void __InitSteppers();
        PositionTrigger __all_position_triggers[HOME_TRIGGER_COUNT];
        FastAccelStepperEngine __stepper_engine = FastAccelStepperEngine();
        FastAccelStepper* __stepper_alpha;
        Led_GPIO __leds[7];
        
};