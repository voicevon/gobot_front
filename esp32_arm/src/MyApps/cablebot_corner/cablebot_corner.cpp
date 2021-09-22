#include "cablebot_corner.h"

CableBotCorner::CableBotCorner(char axis_name){
    this->singleAxis.Name = axis_name;
}
        

void CableBotCorner::HomeAllAxises(){
    // This robot actually is a joint of the Cable-ROBOT system.
    // And, It's necessary to work with other joint cooperately.
    // So, the function "Home()" should do nothing ! 
    this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND); 
}



void CableBotCorner::RunG1(Gcode* gcode){
    float pos = gcode->get_value(singleAxis.Name);
    Serial.println("***************  111");
    bool dir_forward = true;
    if (pos - this->singleAxis._actuator->GetCurrentPos()){
        dir_forward = false;
    }
    //TODO, insert PID here.
    float speed = 100;
    this->objHBridge.Start(speed, dir_forward);
    Serial.println("***************  222");

    //Read the encoder,
    int distance;
    do {
        distance = pos- this->singleAxis._actuator->GetCurrentPos(); 
    }while (distance > 10);
    Serial.println("***************  3333");

}

void CableBotCorner::Init(Encoder* encoder){
    this->LinkCommuDevice(&this->objCommuBle);
    this->objCommuBle.Init();
    this->singleAxis.LinkAcuator(&this->objDcMotor);
    this->objDcMotor.linkSensor(encoder);
    this->objDcMotor.linkDriver(&this->objHBridge);
    this->objDcMotor.MaxSpeed = 100;
    this->commuDevice->OutputMessage("Hello world! This is the first message from commuDevice,");
    this->commuDevice->OutputMessage("    Have a good Day. :) ");
    this->commuDevice->OutputMessage(COMMU_OK);
}

ik_position CableBotCorner::ik(float x, float y){
    ik_position ret;  //TODO: check risk for unreleasing ?
    ret.alpha = x;
    ret.beta = y;   // Will Never useful for me. 
    return ret;
} 
