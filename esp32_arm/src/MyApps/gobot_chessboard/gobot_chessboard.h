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


// #include "accel_stepper/AccelStepper.h"
// #include "accel_stepper/MultiStepper.h"
// #include "mcp23018.h"
#include "actions.h"

// #include "ble_server.h"
#include <ESP32Servo.h>
#include "Robot/RobotArm.h"


#define ARM_ALPHA_AXIS 4
#define ARM_BETA_AXIS 5


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
class GobotChessboard: public RobotArm{
    public:
        static GobotChessboard& getInstance()
        {
            static GobotChessboard instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void Home(unsigned char axis) override;
        void SpinOnce(void);
        void Setup(RobotAction* pAction);

        void SetEffector(EEF action);
        void pick_place_park(RobotAction* pAction);

    private:
        GobotChessboard();
        GobotChessboard(GobotChessboard const& copy);            // Not Implemented
        GobotChessboard& operator=(GobotChessboard const& copy); // Not Implemented

        motor_position ik(float x, float y) override;


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

};
