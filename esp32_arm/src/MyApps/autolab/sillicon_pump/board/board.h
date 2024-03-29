#pragma once

#include "Robot/board/board_base.h"
#include "von/component/display/mono_led_gpio.h"

// #include "MyLibs/component/display/digital_number/hc595_digital_number.h"
#include "Robot/board/display/digital_number/hc595_digital_number.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger_array.h" 
#include "FastAccelStepper.h"
#include "CNC/coordinate/cnc_axis.h"

// #define HOME_TRIGGER_COUNT 0

class Silicon_Pump_Board: public BoardBase{
    public:
        Silicon_Pump_Board();
        void Init(const char* app_welcome_statement) override;
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