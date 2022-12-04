#pragma once

#include "CNC/board/cnc_board_base.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "CNC/Actuator/servo/cnc_actuator_servo.h"
#include "Robot/axis_homer/position_trigger_array.h" 
#include "HX711.h"
#include "VL6180X.h"
#include <ESP32Servo.h>
#include "FastAccelStepper.h"

#define HOME_TRIGGER_COUNT 2
#define SERVO_COUNT 2

class TeethWarehouse_Board: public CncBoardBase{
    public:
        void Init(bool is_on_reset) override;
        
        // We do nothing, just override the methods.
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) override {};
        void EnableVacuumPump(bool enable_it);
        void EnableVacuumSwitch(bool enable_it);
    

        float ReadHx711Adc();
        float ReadVL6180();

        HX711* GetHx711(){return &this->__hx711;};
        FastAccelStepper* GetStepper_Alpha(){return __stepper_alpha;};
        FastAccelStepper* GetStepper_Beta(){return __stepper_beta;};
        Servo* GetServo_onVertical();

        void Test_PositionTriggers(int loops);
        void Test_Hx711(int loops);
        void Test_VL6180x();
        void Test_Servo_AirPen(int loops);
        void Test_Servo_VacuumSwitch(int loops);
        void Test_VacuumPump(int loops);
        // void Test_Servo_Simutenious()

        void Test_SingleStepper(int index, int loops);
        void Test_DualStepper(int loops);


    protected:

    private:

        void __InitSteppers();
        PositionTrigger __all_position_triggers[HOME_TRIGGER_COUNT];
        Servo __all_servos[SERVO_COUNT];
        FastAccelStepperEngine __stepper_engine = FastAccelStepperEngine();
        FastAccelStepper* __stepper_alpha = NULL;
        FastAccelStepper* __stepper_beta = NULL;

        HX711 __hx711;
        VL6180X sensor;
        


        

};