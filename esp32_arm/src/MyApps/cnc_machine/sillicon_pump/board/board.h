#pragma once

#include "MyLibs/board/board_base.h"
#include "MyLibs/component/display/mono_led_gpio.h"
#include "MyLibs/component/display/digital_number/hc595_digital_number.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/axis_homer/position_trigger_array.h" 
#include "FastAccelStepper.h"
#include "CNC/coordinate/cnc_axis.h"

// #define HOME_TRIGGER_COUNT 0

class Sillicon_Pump_Board: public BoardBase{
    public:
        Sillicon_Pump_Board();
        void Init() override;
        
        FastAccelStepper* GetStepper(EnumAxis_Inverseinematic axis);
        // void TurnOn_ThisLed_Only(int led_id);

        // void Test_PositionTriggers(int loops);
        void Test_Stepper(int loops);
        void Test_StepperQueue(int loops);
        // Hc595_Digital_number* GetDisplayer(){return &__displayer;};

    protected:

    private:

        void __InitSteppers();
        // PositionTrigger __all_position_triggers[HOME_TRIGGER_COUNT];
        FastAccelStepperEngine __stepper_engine = FastAccelStepperEngine();
        FastAccelStepper* __stepper_alpha;
        FastAccelStepper* __stepper_beta;
        // Mono_Led_GPIO __leds[7];
        // Hc595_Digital_number __displayer;
        
};