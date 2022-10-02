#pragma once

#include <SimpleFOC.h>
#include "array_base.h"

class PidController_Dictable: public PIDController, public ArrayItem{

};


// Make sure to add an blank constructor to clas  FOC.PidController.
class PidControllers_Diction: public ArrayBase{
    public:
        static PidControllers_Diction& Instance(){
            static PidControllers_Diction __instance;
            return __instance;
        }
        void Init(PidController_Dictable* the_array, int items_count){_Init(the_array, items_count,sizeof(PidController_Dictable));};
        PidController_Dictable* GetPidController(int index){return (PidController_Dictable*)_GetItem(index);};

    private:

};
