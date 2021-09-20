#ifndef _CABLE_BOT_CORNER_H_
#define _CABLE_BOT_CORNER_H_
#include "Robot/RobotBase.hpp"
#include "Robot/Actuator/DCMotor/DcMotor.h"
#include "Robot/Commu/CommuBleGattServer.h"
#include "Robot/Axis/SingleAxisBase.hpp"

template<class Actuator_T,class ActuatorController_T, class ActuatorDriver_T, class Commu_T>
class CableBotCorner: public RobotBase<Actuator_T, ActuatorController_T, ActuatorDriver_T,Commu_T>{
    public:
        CableBotCorner(char axis_name);
        void HomeAllAxises() override;

        // SingleAxisBase<DCMotor> axis = SingleAxisBase<DCMotor>(AXIS_NAME);

};


#endif