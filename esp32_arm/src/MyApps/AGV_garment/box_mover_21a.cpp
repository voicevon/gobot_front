#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "box_mover_21a.h"

BoxMover_21a::BoxMover_21a(){
    // this->singleAxis.Name = axis_name;
    this->objDcMotor.AxisName = 'X';
}
        
void BoxMover_21a::test_hBridge(){
    // for(int i =0 ; i <10; i++){
    //     this->objHBridge.Stop();
    //     delay(5000);

    //     this->objHBridge.Start(255, true);
    //     delay(5000);
    //     this->objHBridge.Start(255, false);
    //     delay(5000);
    // }
}

void BoxMover_21a::test_home(){
    Gcode gc = Gcode("G91");
    this->RunGcode(&gc);
    do{
       if (this->robot_is_idle){
        Gcode gcode = Gcode("G1 A-3");
        this->RunGcode(&gcode);
       }
       delay(1000);
    //    Serial.print(this->singleAxis._actuator->GetCurrentPos());
       Serial.print(this->objDcMotor.GetCurrentPos());
       Serial.print("\n");
    } while (!objHomeTriger.IsTriged());
    
}

void BoxMover_21a::HomeAllAxises(){
    // This robot actually is a joint of the Cable-ROBOT system.
    // And, It's necessary to work with other joint cooperately.
    // So, the function "Home()" should do nothing ! 
    this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND); 
}

std::string BoxMover_21a::GetHomeTrigerStateString(){
    std::string result = "Trigger = ";    // Will be deleted after function run?
    if (this->objHomeTriger.IsTriged()){
        result += "Yes";
    }else{
        result += "No";
    }
    return result;
}

void BoxMover_21a::MoveToTargetPosition(){
    static float debug_last_distance = 0.0f;

    bool dir_forward = true;
    float targetPos = this->nextPosX.x;
    // if (targetPos > this->singleAxis._actuator->GetCurrentPos()){
    if (targetPos > this->objDcMotor.GetCurrentPos()){
        dir_forward = false;
    }
    //TODO, insert PID here.
    float speed = 100;
    // this->objHBridge.Start(speed, dir_forward);
    // this->verticalAxis->Start(speed, dir_forward);
    //Read the encoder,
    // float distance = targetPos - this->singleAxis._actuator->GetCurrentPos(); 
    float distance = targetPos - this->objDcMotor.GetCurrentPos(); 
    if (distance != debug_last_distance){
        Serial.print("\nRunning G1  target, current,   distance to target = ");
        Serial.print(this->nextPosX.x);
        Serial.print("   ");
        // Serial.print(this->singleAxis._actuator->GetCurrentPos());
        Serial.print(this->objDcMotor.GetCurrentPos());
        Serial.print("   ");
        Serial.print(distance);
        debug_last_distance = distance;
    }
    if (abs(distance) < this->objDcMotor.positionTolerance){
        // this->objHBridge.Stop();
        // this->verticalAxis->Stop();
        this->robot_is_idle = true;
    }else{
        this->robot_is_idle = false;
    }
}

void BoxMover_21a::RunG1(Gcode* gcode){
    float pos = gcode->get_value(this->objDcMotor.AxisName);
    // this->singleAxis._actuator->SetTargetAbs(pos);
    if (this->is_absolute_position){
        this->nextPosX.x = pos;
    }else{
        this->nextPosX.x = this->nextPosX.x + pos;
    }
    MoveToTargetPosition();
}
void BoxMover_21a::RunG6(Gcode* gcode){
    float pos = gcode->get_value(this->objDcMotor.AxisName);
    // this->singleAxis._actuator->SetTargetAbs(pos);
    if (this->is_absolute_position){
        this->nextPosX.x = pos;
    }else{
        this->nextPosX.x = this->nextPosX.x + pos;
    }
    do{
        MoveToTargetPosition();
    }while (!this->robot_is_idle);
}

void BoxMover_21a::SpinOnce_BaseExit(){
    this->MoveToTargetPosition();
}

void BoxMover_21a::Init_Linkage(IrEncoderHelper* sensorHelperBase){
    // this->LinkCommuDevice(&this->objCommuBle);
    // this->objCommuBle.Init();
    // this->singleAxis.LinkAcuator(&this->objDcMotor);
    this->objDcMotor.LinkSensorHelper(sensorHelperBase);
    // this->objDcMotor.LinkDriver(&this->objHBridge);
    // this->objDcMotor.LinkDriver(this->verticalAxis);
    this->objDcMotor.AxisName = 'X';
    this->objDcMotor.MaxSpeed = 100;
    this->objDcMotor.positionTolerance = 1.5f;

    this->commuDevice->OutputMessage("Hello world! This is the first message from commuDevice,");
    this->commuDevice->OutputMessage("    Have a good Day. :) ");
    this->commuDevice->OutputMessage(COMMU_OK);

}

// ik_position BoxMover_21a::ik(float x, float y){
//     ik_position ret;  //TODO: check risk for unreleasing ?
//     ret.alpha = x;
//     ret.beta = y;   // Will Never useful for me. 
//     return ret;
// } 


IkPositionBase* BoxMover_21a::IK(FkPositionBase* fk){
    FkPosX* _fk = (FkPosX*)(fk);
    this->objIkPos.alpha = _fk->x;
    return &this->objIkPos;  
}

FkPositionBase* BoxMover_21a::FK(IkPositionBase* ik){
    IkPosX* _ik = (IkPosX*)(ik);
    this->objFkpos.x = _ik->alpha;
    return &this->objFkpos;
}

#endif