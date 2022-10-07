#pragma once

// #include "MyLibs/dictions/diction_base.h"
#include "cnc_actuator_base.h"
#include "dc_motor/actuator_dc_motor.h"


class CncActuator_List: public ListBase{
    public:
        static CncActuator_List& Instance(){
            static CncActuator_List __instance;
            return __instance;
            };

        void Init(CncActuatorBase** all_items, int count){
            _Init((ListItem**)all_items, count);
            };
        void AddActuator(CncActuatorBase* actuator){
            __AddItem(actuator);
            };
        CncActuatorBase* GetActuator(int index) {
            return (CncActuatorBase*)_GetItem(index);
            };
        
        // This is risky,  Be careful;
        ActuatorDcMotor* GetActuator_DcMotor(int index) {return (ActuatorDcMotor*)_GetItem(index);};
};