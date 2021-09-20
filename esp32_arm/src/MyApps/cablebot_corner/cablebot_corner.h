#ifndef _CABLE_BOT_CORNER_H_
#define _CABLE_BOT_CORNER_H_
#include "Robot/SingleAxisRobot.hpp"
#include "Robot/Actuator/DCMotor/DcMotor.h"
#include "Robot/Commu/CommuBleGattServer.h"

template<class Actuator_T,class ActuatorController_T, class Commu_T>
class CableBotCorner: public SingleAxisRobot<Actuator_T, ActuatorController_T, Commu_T>{
    public:
        CableBotCorner(char axis_name);
        void Home() override;

        // SingleAxis<DCMotor> axis = SingleAxis<DCMotor>(AXIS_NAME);

};


#endif