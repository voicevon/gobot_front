#pragma once
/*
Stepper liberys:

    https://github.com/gin66/FastAccelStepper
    https://github.com/luni64/TeensyStep
       * Does't support ESP32
    - https://github.com/RCP1/ESP32Step
    https://github.com/Stan-Reifel/FlexyStepper
    - https://github.com/pkerspe/ESP-FlexyStepper
*/

#define PIN_LED_POWER_2112 12
#define PIN_LED_B_2112 25
#define PIN_LED_C_2112 26
#define PIN_LED_F_2112 27
#define PIN_LED_H_2112 14

#define PIN_HOME_ALHPA_2112  35     //??
#define PIN_ALPHA_DIR_2112 19
#define PIN_ALPHA_STEP_2112 5
#define PIN_ALPHA_ENABLE_2112 18

#define PIN_HOME_BETA_2112  34      //??
#define PIN_BETA_DIR_2112 17
#define PIN_BETA_STEP_2112 4
#define PIN_BETA_ENABLE_2112 16

// Total occupied 10 Pins
#define PIN_EEF_SERVO_2112 21
#define PIN_EEF_A_2112  18
#define PIN_EEF_B_2112  19


#define PIN_ENDER_COIL_2112 32
#define PIN_ENDER_COIL_EXT_2112 33

#define ALPHA_HOME_POSITION_2112 2.2
#define BETA_HOME_POSITION_2112 3.3



//  unit is mm
#define LINK_0_2112 191.0  // Length between origin and the two motors
#define LINK_A_2112 285.18 // Length from motor to passive joints
#define LINK_B_2112 384.51 // Length from passive joints to end effector


#define HOMED_POSITION_ALPHA_2112 123   // unit is step
#define HOMED_POSITION_BETA_2112 112    // unit is step

#define STEPS_PER_RAD_2112 3056     // 200 * 16 * 6 / (3.1415927 * 2)
#define MOTOR_MAX_SPEED_2112 1000    // unit?


#include "actions.h"
#include <ESP32Servo.h>
#include "Robot/RobotBase.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/Commu/CommuBleGattServer.h"
#include "Robot/Commu/CommuUart.h"
#include "MyLibs/MyFunctions.hpp"
#include "Robot/Gcode.h"



// #include "ESP32Step/src/TeensyStep.h"
#include "Robot/HomeHelper.h"
#include "MyLibs/Components/Led.h"
// Up to 10 steppers can be handled as a group by MultiStepper

enum EEF{
    Lower = 1,
    Higher = 2,
    Suck = 3,
    Release = 4,
    Sleep = 5
};


/*
What I know:    
        X,Y position.

What I don't know:
        Chessboard size, and where is chessboard. 
        Stone point position,
        Trashbin position,

How to solve the concepts I don't know?
    Please send X,Y position to me.
    Don't send me nominal name of go game (either of any other name).

*/
class GobotChessboard: public RobotBase{
    public:
        static GobotChessboard& getInstance()
        {
            static GobotChessboard instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void HomeAllAxises() override;
        void RunG1(Gcode* gcode) override;
        void Init() override;
        void SpinOnce(void);
        void Setup(RobotAction* pAction);

        void SetEffector(EEF action);
        void pick_place_park(RobotAction* pAction);
    
    private:
        GobotChessboard();
        GobotChessboard(GobotChessboard const& copy);            // Not Implemented
        GobotChessboard& operator=(GobotChessboard const& copy); // Not Implemented

        ik_position ik(float x, float y) override;

        // SingleAxisBase<Stepper> obj_axis_alpha = SingleAxisBase<Stepper>('A');
        // SingleAxisBase<Stepper> obj_axis_beta = SingleAxisBase<Stepper>('B');

        // link length in mm
        int link_0 ;   // Length between origin and the two motors
        int link_a ;   // Length from motor to passive joints
        int link_b ;  // Length from passive joints to end effector


        // EG X-Y position bed driven by 2 steppers
        // Alas its not possible to build an array of these with different pins for each :-(
        // AccelStepper stepper_alpha(AccelStepper::MotorInterfaceType::FULL4WIRE, 6, 7, 8, 9,true);
        int STEPS_PER_RAD;
        // MultiStepper steppers;


        Servo* eefServo;

        void __HomeSpin(Stepper* homing_stepper, uint8_t home_pin);
        bool homed;

        RobotAction* __arm_action;


    protected:
    private:
        Led objLedPower = Led(0, PIN_LED_POWER_2112, LOW);
        Led objLedHome_alpha = Led(1,2,LOW);
        HomeHelper objHomeHelper_alpha = HomeHelper(PIN_HOME_ALHPA_2112, HIGH);
        HomeHelper objHomeHelper_beta = HomeHelper(PIN_HOME_BETA_2112, HIGH);

        SingleAxis objAxis_Alpha = SingleAxis();
        SingleAxis objAxis_Beta = SingleAxis();
        ActuatorBase objActuator_Alpha = ActuatorBase();
        ActuatorBase objActuator_Beta = ActuatorBase();

        Stepper objStepper_alpha = Stepper(PIN_ALPHA_STEP_2112, PIN_ALPHA_DIR_2112);
        Stepper objStepper_beta = Stepper(PIN_BETA_STEP_2112, PIN_BETA_DIR_2112);
        StepControl objStepControl;

        CommuBleGattServer objCommuBle = CommuBleGattServer();
        CommuUart objCommuUart = CommuUart();
};


