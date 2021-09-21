#include "cablebot_corner.h"

CableBotCorner::CableBotCorner(char axis_name){
    // SingleAxisRobot<Actuator_T>::_Axis_Name = axis_name;
    // this->axi_Axis_Name = axis_name;
}
        

void CableBotCorner::HomeAllAxises(){
    // This robot actually is a joint of the Cable-ROBOT system.
    // And, It's necessary to work with other joint cooperately.
    // So, the function "Home()" should do nothing ! 
    this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND); 
}

void CableBotCorner::SpinOnce(){
    this->_base_spin_once();
}

void CableBotCorner::RunG1(Gcode* gcode){
    float pos = gcode->get_value(singleAxis._Axis_Name);
    bool dir_forward = true;
    if (pos - this->singleAxis._actuator->GetCurrentPos()){
        dir_forward = false;
    }
    //TODO, insert PID here.
    float speed = 100;
    this->hBridge->Start(speed, dir_forward);

    //Read the encoder,
    int distance;
    do {
        distance = pos- this->singleAxis._actuator->GetCurrentPos(); 
    }while (distance > 10);

}

void CableBotCorner::Init(){
        this->LinkCommuDevice(&this->commuUart);
        this->AppendAxis(&this->singleAxis);
        // this->singleAxis.LinkAcuator(&this->hBridge);


        Led* ledPower = &objLed_power;
        Led* ledHomeAlpha = &objLed_home_alpha; 
        DCDriverHBridge* hBridge =&objBridge;
        HomeTriger* homeTriger= &objHomeTriger; 
        DCMotor* motor = &objMotor; 
}