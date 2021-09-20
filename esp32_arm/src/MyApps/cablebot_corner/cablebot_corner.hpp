#include "cablebot_corner.h"

template<class Actuator_T, class ActuatorController_T, class ActuatorDriver_T,class Commu_T>
CableBotCorner<Actuator_T,ActuatorController_T, ActuatorDriver_T, Commu_T>::CableBotCorner(char axis_name){
    // SingleAxisRobot<Actuator_T>::_Axis_Name = axis_name;
    // this->axi_Axis_Name = axis_name;
}
        

template<class Actuator_T, class ActuatorController_T, class ActuatorDriver_T, class Commu_T>
void CableBotCorner<Actuator_T,ActuatorController_T, ActuatorDriver_T, Commu_T>::HomeAllAxises(){
    // This robot actually is a joint of the Cable-ROBOT system.
    // And, It's necessary to work with other joint cooperately.
    // So, the function "Home()" should do nothing ! except the response
    this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND); 
}