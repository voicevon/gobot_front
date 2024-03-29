#pragma once

#include "Robot/board/board_base.h"
#include "von/component/display/mono_led_gpio.h"

#include "Robot/board/display/digital_number/hc595_digital_number.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger_array.h" 
// #include "FastAccelStepper.h"

#define HOME_TRIGGER_COUNT 1

class Twh_LoopPorter_Board: public BoardBase{
    public:
        Twh_LoopPorter_Board();
        void Init(const char* app_welcome_statement) override;
        
        FastAccelStepper* GetStepper(){return __stepper_alpha;};
        void TurnOn_ThisLed_Only(int led_id);

        void Test_Stepper(int loops);
        void Test_StepperQueue(int loops);

        
        Hc595_Digital_number* GetNumberDisplayer(){return &__displayer;};
        Mono_Led_GPIO* GetLed(int index){return &__leds[index];};

        void TestLeds(int loops);



    protected:

    private:

        void __InitSteppers();
        PositionTrigger __all_position_triggers[HOME_TRIGGER_COUNT];
        FastAccelStepperEngine __stepper_engine = FastAccelStepperEngine();
        FastAccelStepper* __stepper_alpha;
        Hc595_Digital_number __displayer;

        Mono_Led_GPIO __leds[7];
        
};