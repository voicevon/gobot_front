#pragma once

// #include "MyLibs/dictions/diction_base.h"
#include "actuator_base.h"


class Actuator_Diction: public ArrayBase{
    public:
        static Actuator_Diction& Instance(){
            static Actuator_Diction __instance;
            return __instance;
        };
        ActuatorBase* GetActuator(int index) {return (ActuatorBase*)_GetItem(index);};
};