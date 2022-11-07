#pragma once


#include "CNC/board/cnc_board_base.h"
// #include "CNC/Actuator/stepper/actuator_stepper.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "CNC/Actuator/servo/cnc_actuator_servo.h"
#include "Robot/axis_homer/home_trigger_array.h" 
#include "HX711.h"
#include "VL6180X.h"
#include <ESP32Servo.h>
// #include "ESP32Step/src/TeensyStep.h"
#include "FastAccelStepper.h"


#define HOME_TRIGGER_COUNT 3




class TeethWarehouse_Board: public CncBoardBase{
    public:
        // For being a real PCB board.
        // TeethWarehouse_Board(){};
        void Init(bool is_on_reset) override;
        

        // For being an actuator and its components.

        // For being a CNC machine.
        // RobotEefBase* GetEef() override {return &this->__eef;};

        // We do nothing, just override the methods.
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) override {};
        void EnableVacuumPump(bool enable_it);
        // void EnableVacuumeSucker(bool enable_it);
    

        float ReadHx711Adc();
        float ReadVL6180();

        HX711* GetHx711(){return &this->__hx711;};
        FastAccelStepper* GetStepper_Alpha(){return __stepper_alpha;};
        FastAccelStepper* GetStepper_Beta(){return __stepper_beta;};

        void Test_PositionTriggers(int loops);
        void Test_Hx711(int loops);
        void Test_VL6180x();
        void Test_Servo_AirPen(int loops);
        void Test_Servo_AirSwitch(int loops);
        void Test_VacuumPump(int loops);
        // void Test_Servo_Simutenious()

        void Test_SingleStepper(int index, int loops);
        void Test_DualStepper(int loops);


    protected:

    private:

        void __InitSteppers();
        PositionTrigger __all_position_triggers[HOME_TRIGGER_COUNT];
        FastAccelStepperEngine __stepper_engine = FastAccelStepperEngine();
        FastAccelStepper* __stepper_alpha = NULL;
        FastAccelStepper* __stepper_beta = NULL;
        

        HX711 __hx711;
        // VL6180x sensor(VL6180X_ADDRESS);
        VL6180X sensor;
        Servo __servo_air_switch;
        Servo __servo_air_pen;

        // CncActuatorServo servo_air_pen;
        


        

};