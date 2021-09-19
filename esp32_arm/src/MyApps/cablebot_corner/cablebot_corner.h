#ifndef _CABLE_BOT_CORNER_H_
#define _CABLE_BOT_CORNER_H_
#include "Robot/SingleAxisRobot.hpp"
#include "Robot/Actuator/DcMotor.h"
#include "Robot/Commu/CommuBleGattServer.h"

template<class Actuator_T>
class CableBotCorner: public SingleAxisRobot<Actuator_T,CommuBleGattServer>{
    public:
        CableBotCorner(char axis_name);
        void Home() override;

        // SingleAxis<DCMotor> axis = SingleAxis<DCMotor>(AXIS_NAME);

};


#endif