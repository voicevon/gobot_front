#include "cablebot_corner.h"

template<class Actuator_T>
CableBotCorner<Actuator_T>::CableBotCorner(char axis_name){
    // SingleAxisRobot<Actuator_T>::_Axis_Name = axis_name;
    this->_Axis_Name = axis_name;
}
        

template<class Actuator_T>
void CableBotCorner<Actuator_T>::Home(){
    // This robot actually is a joint of the Cable-ROBOT system.
    // And, It's necessary to work with other joint cooperately.
    // So, the function "Home()" should do nothing ! except the response
    this->__output_message(COMMU_UNKNOWN_COMMAND); 
}