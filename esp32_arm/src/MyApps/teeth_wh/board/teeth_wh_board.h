#pragma once


#include "CNC/board/cnc_board_base.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
#include "CNC/Actuator/servo/cnc_actuator_servo.h"
#include "Robot/axis_homer/home_trigger_array.h" 
#include "HX711.h"
#include "VL6180X.h"
#include <ESP32Servo.h>

#define HOME_TRIGGER_COUNT 3

#define PIN_ALPHA_DIR 14
#define PIN_ALPHA_STEP 12
#define PIN_BETA_DIR 26
#define PIN_BETA_STEP 27
#define PIN_STEPPER_ENABLE 13


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
        void EnableVacuumePump(bool enable_it);
        // void EnableVacuumeSucker(bool enable_it);
    

        float ReadHx711Adc();
        float ReadVL6180();

        HX711* GetHx711(){return &this->__hx711;};
        Stepper* GetStepper_Alpha(){return &this->__stepper_alpha;};
        Stepper* GetStepper_Beta(){return &this->__stepper_beta;};

        void Test_PositionTriggers(int loops);
        void Test_Hx711(int loops);
        void Test_VL6180x();
        void Test_Servo(int loops);


    protected:

    private:
        PositionTrigger __all_position_triggers[HOME_TRIGGER_COUNT];
        // PositionTrigger __all_position_triggers[3];
        Stepper __stepper_alpha = Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
        Stepper __stepper_beta = Stepper(PIN_BETA_STEP, PIN_BETA_DIR);
        

        HX711 __hx711;
        // VL6180x sensor(VL6180X_ADDRESS);
        VL6180X sensor;
        Servo __servo_air_switch;
        Servo __servo_air_pen;

        // CncActuatorServo servo_air_pen;
        


        

};