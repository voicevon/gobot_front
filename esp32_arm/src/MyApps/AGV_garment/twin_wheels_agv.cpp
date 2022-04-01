#include "twin_wheels_agv.h"


#define HCSR04_PIN_ECHO 27
#define HCSR04_PIN_TRIG 28

#define LEFT_APDS_9960_SDA 21
#define LEFT_APDS_9960_SCL 22
#define RIGHT_APDS_9960_SDA 23
#define RIGHT_APDS_9960_SCL 15


TwinWheelsAgv::TwinWheelsAgv(){

} 

void TwinWheelsAgv::Init(){
    this->trackSensor = new TrackSensor_Dual9960(LEFT_APDS_9960_SDA, LEFT_APDS_9960_SCL, RIGHT_APDS_9960_SDA, RIGHT_APDS_9960_SCL);
	this->obstacleSensor = new UltraSonicDistanceSensor(HCSR04_PIN_TRIG, HCSR04_PIN_ECHO);
    this->leftWheel_commu = &Serial1;
    this->rightWheel_commu = &Serial2;
}

void TwinWheelsAgv::SpinOnce(){
    // Test sensor
    if (true){
        this->trackSensor->IsFollowingLeft = ! this->trackSensor->IsFollowingLeft;
        float xx_error = this->trackSensor->ReadForwardingError();
        Serial.println(xx_error);
    }

    // Obstacle detection
    bool found_obstacle = false;
    float distance_to_obstacle =  this->obstacleSensor->measureDistanceCm(); 
    if (distance_to_obstacle >0 && distance_to_obstacle <50) 
        found_obstacle = true;

    // bool found_slowdown_mark = false;     // from track sensor
    int16_t x_error = 0;
    int16_t y_error = 0;
    // bool follow_left = true;
    // bool going_on_fast_moving = true;
    //    this->onMqttReceived();
    //    this->objBoxMover.SpinOnce();

   switch (this->_State) {
   case FAST_MOVING:
        if (found_obstacle)
            this->ToState(FAST_MOVING_PAUSED);
        // check if see the mark of slow-down.
        else if (this->trackSensor->GetFlag_Slowdown()){
            this->ToState(SLOW_MOVING);
            this->trackSensor->ClearFlag_Slowdown();
        }else{
            //going on with fast_moving.
            x_error = this->trackSensor->ReadForwardingError();
            this->MovingLoop(x_error);
        }
        break;
    case FAST_MOVING_PAUSED:
        if (!found_obstacle)
            this->ToState(FAST_MOVING);
        break;
    case SLOW_MOVING:
        if (found_obstacle)
            this->ToState(SLOW_MOVING_PAUSED);
        else {
            //try to read RFID, have read already.
            // track_node_id = this->rfidReader->PICC_ReadCardSerial(); 
            // got the site_id, we will know should follow left or follow right.
            // this->__current_navigator_point = this->objMapNavigator.FetchSite(mapsite_id);
            // if (this->__current_navigator_point.GoingOnFollowLeft()){
            //     this->trackSensor->__folking = TrackSensor_Dual9960::FOLKING::FOLLOWING_LEFT;
            // }else{
            //     this->trackSensor->__folking = TrackSensor_Dual9960::FOLKING::FOLLOWING_RIGHT;
            // }
            // if (this->__current_navigator_point.GoingOnFastMoving()){
            //     this->ToState(FAST_MOVING);
            // }else{
            //     this->ToState(PARKING);
            // }
        }
        break;
   case SLOW_MOVING_PAUSED:
      if (!found_obstacle)
         this->ToState(SLOW_MOVING);
      break;
   case PARKING:
        // try to finish parking
        if(this->DoParking()){
            this->ToState(PARKED);
        }
        break;
   case PARKING_PAUSED:
        if (!found_obstacle)
            this->ToState(PARKING);
            break;
   case PARKED:
        break;
   
   default:
      break;
   }
}

void TwinWheelsAgv::ToState(AGV_STATE state){
    if (state == this->_State) return;
    switch(state){
    case FAST_MOVING:
        this->common_speed = this->__fast_velocity;
        break;
    case SLOW_MOVING:
        this->common_speed = this->__slow_velocity;
        break;
    case PARKING:
        this->common_speed = this->__parking_velocity;
        break;
    case FAST_MOVING_PAUSED:
        this->leftWheel_commu->write("T0");
        this->rightWheel_commu->write("T0");
        break;
    case SLOW_MOVING_PAUSED:
        this->leftWheel_commu->write("T0");
        this->rightWheel_commu->write("T0");
        break;
    case PARKED:
        this->leftWheel_commu->write("T0");
        this->rightWheel_commu->write("T0");
        break;
        
    default:
        Serial.print("[Warn] TwinWheelsAgv::ToState()  switch without case ");
        Serial.println(state);
        break;
    }
    this->_State = state;
    Serial.print("\n GarmentBot::SetMode()  " );
    Serial.println(state);

   
}

// return true:   reach parked position
// return false:  still parking
bool TwinWheelsAgv::DoParking(){
    int16_t x_error ;
    int16_t y_error ;
    this->trackSensor->ReadParkingError(&x_error, &y_error);

    this->common_speed =  this->__parking_velocity;
    if (x_error < 10 && y_error < 10)
        return true;
    return false;
}