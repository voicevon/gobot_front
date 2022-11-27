#pragma once

// #include <SimpleFOC.h>
#include "list_base.h"

class RosNode{

};

class RosNode_Listable: public RosNode, public ListItem{
    public:
        RosNode_Listable(): RosNode(){};
};


class PidControllers_List: public ListBase{
    public:
        static PidControllers_List& Instance(){
            static PidControllers_List __instance;
            return __instance;
        }

        void Init(RosNode_Listable** the_list, int items_count){
            Logger::Print("Init()",1);
            _Init((ListItem**)the_list, items_count);
            };
        void AddPidController(RosNode_Listable* pid){__AddItem(pid);};
        RosNode_Listable* GetPidController(int index){return (RosNode_Listable*)_GetItem(index);};

    private:

};
