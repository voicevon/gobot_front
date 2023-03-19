#pragma once

#include "MyLibs/board/board_base.h"
#include "MyLibs/component/display/mono_led_gpio.h"
#include "MyLibs/component/display/digital_number/hc595_digital_number.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/axis_homer/position_trigger_array.h" 
#include "FastAccelStepper.h"
#include "CNC/coordinate/cnc_axis.h"

// #define HOME_TRIGGER_COUNT 0

class Silicon_Pump_Board: public BoardBase{
    public:
        Silicon_Pump_Board();
        void Init() override;
        FastAccelStepper* GetStepper(EnumAxis_Inverseinematic axis);

        void Test_Stepper(int loops);
        void Test_StepperQueue(int loops);
        Mono_Led_GPIO* GetStateLed(){return & __state_led;};

    protected:

    private:
        void __InitSteppers();
        FastAccelStepperEngine __stepper_engine = FastAccelStepperEngine();
        FastAccelStepper* __stepper_alpha;
        FastAccelStepper* __stepper_beta;
        FastAccelStepper* __stepper_gama;
        Mono_Led_GPIO __state_led;
        
};