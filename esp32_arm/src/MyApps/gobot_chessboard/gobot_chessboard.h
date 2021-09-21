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

#define PIN_LED_POWER 12
#define PIN_LED_B 25
#define PIN_LED_C 26
#define PIN_LED_F 27
#define PIN_LED_H 14

#define PIN_HOME_ALHPA 35     //??
#define PIN_ALPHA_DIR 19
#define PIN_ALPHA_STEP 5
#define PIN_ALPHA_ENABLE 18

#define PIN_HOME_BETA 34      //??
#define PIN_BETA_DIR 17
#define PIN_BETA_STEP 4
#define PIN_BETA_ENABLE 16

#define ENDER_COIL 32
#define ENDER_COIL_EXT 33

#include "actions.h"
#include <ESP32Servo.h>
#include "Robot/RobotBase.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/Commu/CommuBleGattServer.h"
#include "MyLibs/MyFunctions.hpp"
#include "Robot/Gcode.h"
#define ARM_ALPHA_AXIS 4
#define ARM_BETA_AXIS 5


// #include "ESP32Step/src/TeensyStep.h"
#include "Robot/HomeTriger.h"
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
        Led led_power = Led(0, PIN_LED_POWER, LOW);
        // led_Robot = Led(2,1,LOW);
        Led led_home_alpha = Led(1,2,LOW);
        HomeTriger homeTriger_alpha = HomeTriger(PIN_HOME_ALHPA, HIGH);
        HomeTriger homeTriger_beta = HomeTriger(PIN_HOME_BETA, HIGH);

        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP, PIN_BETA_DIR);
        StepControl stepControl;
};
