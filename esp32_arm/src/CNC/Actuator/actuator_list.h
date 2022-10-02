#pragma once

// #include "MyLibs/dictions/diction_base.h"
#include "actuator_base.h"
#include "dc_motor/actuator_dc_motor.h"


class Actuator_List: public ListBase{
    public:
        static Actuator_List& Instance(){
            static Actuator_List __instance;
            return __instance;
            };

        void Init(ActuatorBase** all_items, int count){
            _Init((ListItem**)all_items, count);
            };
        void AddActuator(ActuatorBase* actuator){
            __AddItem(actuator);
            };
        ActuatorBase* GetActuator(int index) {
            return (ActuatorBase*)_GetItem(index);
            };
        
        // This is risky,  Be careful;
        ActuatorDcMotor* GetActuator_DcMotor(int index) {return (ActuatorDcMotor*)_GetItem(index);};
};