#pragma once

#include <SimpleFOC.h>
// #include "array_base.h"
#include "list_base.h"

class PidControllers_Listable: public PIDController, public ListItem{
    public:
        PidControllers_Listable(float P, float I, float D, float ramp, float limit): PIDController(P,I,D,ramp,limit){};
};


class PidControllers_List: public ListBase{
    public:
        static PidControllers_List& Instance(){
            static PidControllers_List __instance;
            return __instance;
        }

        void Init(PidControllers_Listable** the_list, int items_count){_Init((ListItem**)the_list, items_count);};
        void AddPidController(PidControllers_Listable* pid){__AddItem(pid);};
        PidControllers_Listable* GetPidController(int index){return (PidControllers_Listable*)_GetItem(index);};

    private:

};
