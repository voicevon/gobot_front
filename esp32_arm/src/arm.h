#pragma once

#include "accel_stepper/AccelStepper.h"
#include "accel_stepper/MultiStepper.h"
#include "actions.h"
#include "ble_server.h"
#include <ESP32Servo.h>>

// struct point_position{
//     int x;
//     int y;
// };
struct motor_position{
    int alpha;
    int beta;
};


// Up to 10 steppers can be handled as a group by MultiStepper

enum EEF{
    Lower = 1,
    Higher = 2,
    Suck = 3,
    Release = 4,
    Sleep = 5
};

class Arm{
    public:
         static Arm& getInstance()
        {
            static Arm instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void Home(unsigned char axis);
        void Init(void);
        void SpinOnce(ArmAction action);
        void pick_place_park(ArmAction arm_action);

    private:
        Arm();

        Arm(Arm const& copy);            // Not Implemented
        Arm& operator=(Arm const& copy); // Not Implemented
        void MoveTo(int x, int y);
        void SetEffector(EEF action);
        void pick_place_park(ArmAction* arm_action);

        // home pin defination
        unsigned int alpha_home_pin = 23;
        unsigned int beta_home_pin = 24;
        // link length in mm
        int l0 = 4.05;   // Length between origin and the two motors
        int l1 = 8.05;   // Length from motor to passive joints
        int l2 = 12.05;  // Length from passive joints to end effector


        // EG X-Y position bed driven by 2 steppers
        // Alas its not possible to build an array of these with different pins for each :-(
        AccelStepper stepper_alpha;
        AccelStepper stepper_beta;
        MultiStepper steppers;
        Servo leftServo;
        Servo rigtServo;
        motor_position ik(int x, int y);
        BleServer* __ble_server;
    

    protected:

};
