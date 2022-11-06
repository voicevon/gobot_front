#pragma once
#include "FastAccelStepper.h"
#include "ESP32Step/src/TeensyStep.h"
#include "MyLibs/basic/logger.h"


class TeensyStep_Gateway{
    public:
        static TeensyStep_Gateway& Instance(){
            static TeensyStep_Gateway __instance;
            return __instance;
        };

        void Init(StepControl* step_control){
            __step_control = step_control;
        };

        void AddStepper(Stepper* new_stepper){
            switch (__steppers_count){
                case 0:
                    stepper_a = new_stepper;
                    break;
                case 1:
                    stepper_b = new_stepper;
                    break;
                case 2:
                    stepper_g = new_stepper;
                    break;
                case 3:
                    stepper_d = new_stepper;
                    break;
            
            default:
                break;
            }
            __steppers_count++;
        };
        // void LinkDualStepper(Stepper* stepper1, Stepper* stepper2);
        // void LinkTrippleStepper(Stepper* stepper1, Stepper* stepper2,Stepper* stepper3);

        void AsyncMove(){
            Logger::Debug("TeensyStep_Gateway::AsyncMove()");
            Logger::Print("__steppers_count", __steppers_count);
            switch (__steppers_count){
                case 1: __step_control->moveAsync(*stepper_a); break;
                case 2: __step_control->moveAsync(*stepper_a,*stepper_b); break;
                case 3: __step_control->moveAsync(*stepper_a,*stepper_b, *stepper_g); break;
                case 4: __step_control->moveAsync(*stepper_a,*stepper_b, *stepper_g,*stepper_d); break;
                default: break;
            }
        };

    private:
        int __steppers_count = 0;
        StepControl* __step_control;
        Stepper* stepper_a;
        Stepper* stepper_b;
        Stepper* stepper_g;
        Stepper* stepper_d;
        Stepper* stepper_e;
        Stepper* stepper_z;
};