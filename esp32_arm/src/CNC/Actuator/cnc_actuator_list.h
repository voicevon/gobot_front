#pragma once

#include "cnc_actuator_base.h"

class gs_CncActuator_List: public ListBase{
    public:
        static gs_CncActuator_List& Instance(){
            static gs_CncActuator_List __instance;
            return __instance;
            };

        void Init(CncActuatorBase** all_items, int count){
            _Init((ListItem**)all_items, count);
            };

        void AddActuator(CncActuatorBase* actuator){
            __AddItem(actuator);
            };

        bool HasMovingActuator(){
            CncActuatorBase* actuator;
            for(int a=0; a<this->GetItemsCount(); a++){
                actuator = this->GetActuator(a);
                if (actuator->IsMoving()){
                    // Logger::Print("gs_CncActuator_List::HasMovingActuator()", actuator->MyName);
                    return true;
                }        
            }

            return false;
            };
        
        CncActuatorBase* GetActuator(int index) {
            return (CncActuatorBase*)_GetItem(index);
            };
        // This is risky,  Be careful;
        // CncActuatorDcMotor* GetActuator_DcMotor(int index) {return (CncActuatorDcMotor*)_GetItem(index);};
};