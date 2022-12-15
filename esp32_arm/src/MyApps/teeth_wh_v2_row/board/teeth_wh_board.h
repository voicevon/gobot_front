#pragma once

#include "CNC/board/cnc_board_base.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/axis_homer/position_trigger_array.h" 
#include "FastAccelStepper.h"

#define HOME_TRIGGER_COUNT 1

class Twh2Row_Board: public CncBoardBase{
    public:
        void Init(bool is_on_reset) override;
        
        float ReadVL6180();

        // HX711* GetHx711(){return &this->__hx711;};
        FastAccelStepper* GetStepper(){return __stepper_alpha;};

        void Test_PositionTriggers(int loops);
        void Test_Stepper(int loops);


    protected:

    private:

        void __InitSteppers();
        PositionTrigger __all_position_triggers[HOME_TRIGGER_COUNT];
        FastAccelStepperEngine __stepper_engine = FastAccelStepperEngine();
        FastAccelStepper* __stepper_alpha;
        


        

};