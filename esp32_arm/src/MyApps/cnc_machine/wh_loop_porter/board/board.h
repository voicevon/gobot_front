#pragma once

#include "MyLibs/board/board_base.h"
#include "MyLibs/component/display/mono_led_gpio.h"
#include "MyLibs/component/display/digital_number/hc595_digital_number.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/axis_homer/position_trigger_array.h" 
#include "FastAccelStepper.h"

#define HOME_TRIGGER_COUNT 1

class Twh_LoopPorter_Board: public BoardBase{
    public:
        Twh_LoopPorter_Board();
        void Init() override;
        
        FastAccelStepper* GetStepper(){return __stepper_alpha;};
        void TurnOn_ThisLed_Only(int led_id);

        void Test_Stepper(int loops);
        void Test_StepperQueue(int loops);

        
        Hc595_Digital_number* GetDisplayer(){return &__displayer;};
        Mono_Led_GPIO* GetLed(int index){return &__leds[index];};

        void TestLeds(int loops);



    protected:

    private:

        void __InitSteppers();
        PositionTrigger __all_position_triggers[HOME_TRIGGER_COUNT];
        FastAccelStepperEngine __stepper_engine = FastAccelStepperEngine();
        FastAccelStepper* __stepper_alpha;
        Mono_Led_GPIO __leds[7];
        Hc595_Digital_number __displayer;
        
};