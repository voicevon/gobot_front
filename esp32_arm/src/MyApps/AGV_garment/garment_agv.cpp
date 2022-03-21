#include "garment_agv.h"

GarmentAgv::GarmentAgv(){
    this->trackSensor = new TrackSensor_Dual9960(1,2,3,4);
    this->obstacleSensor = new UltraSonicDistanceSensor(1,2);
    this->rfidReader = new MFRC522(1,2);
} 

void GarmentAgv::SpinOnce(){
    // int distance_to_full_park = 100;      //???
    bool loading_finished = true;         // From mqtt
    bool unloading_finished = true;       // from mqtt
    uint16_t track_node_id = 0;               // read from RFID

    // Obstacle detection
    this->found_obstacle = false;
    float distance =  this->obstacleSensor->measureDistanceCm(); 
    if (distance >0 && distance <50) 
        this->found_obstacle = true;


    // bool found_slowdown_mark = false;     // from track sensor
    int track_error = this->trackSensor->ReadError_LeftRight();
    int position_error = this->trackSensor->ReadError_FrontRear();
    // bool follow_left = true;
    // bool going_on_fast_moving = true;
    //    this->onMqttReceived();
    //    this->objBoxMover.SpinOnce();

   switch (this->_State) {
   case FAST_MOVING:
        if (found_obstacle)
            this->ToState(FAST_MOVING_PAUSED);
        // check if see the mark of slow-down.
        else if (track_error)
            this->ToState(SLOW_MOVING);
        else
            this->MoveForward(track_error);
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
            track_node_id = this->rfidReader->PICC_ReadCardSerial(); 
            // got the site_id, we will know should follow left or follow right.
            // this->__current_navigator_point = this->objMapNavigator.FetchSite(mapsite_id);
            if (this->__current_navigator_point.GoingOnFollowLeft()){
                this->trackSensor->__folking = TrackSensor_Dual9960::FOLKING::FOLLOWING_LEFT;
            }else{
                this->trackSensor->__folking = TrackSensor_Dual9960::FOLKING::FOLLOWING_RIGHT;
            }
            if (this->__current_navigator_point.GoingOnFastMoving()){
                this->ToState(FAST_MOVING);
            }else{
                this->ToState(PARKING);
            }
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
        if (this->__current_navigator_point.LOADING)
            this->ToState(LOADING);
        else if (this->__current_navigator_point.UNLOADING)
            this->ToState(UNLOADING);
        else if (this->__current_navigator_point.SLEEPING)
            this->ToState(SLEEPING);
        else if (this->__current_navigator_point.CHARGING)
            this->ToState(CHARGING);
        break;
   case LOADING:
        if (loading_finished)
            this->ToState(FAST_MOVING);
        break;
   case UNLOADING:
      if (unloading_finished)
         this->ToState(FAST_MOVING);
      break;
   case SLEEPING:
      /* code */
      break;
   
   default:
      break;
   }
}

void GarmentAgv::ToState(GARMENTAGV_STATE state){
   if (state == this->_State) return;
   switch(state){
      case SLEEPING:
            this->Stop();
            break;
      case FAST_MOVING:
            this->SetTargetSpeed(220);
            break;
      case SLOW_MOVING:
            this->SetTargetSpeed(100);
            break;
      case PARKING:
            this->SetTargetSpeed(20);
            break;
      case LOADING:
            // this->LoadBox();
            break;
      case UNLOADING:
            // this->objBoxMover.UnloadBox();
            break;
      case CHARGING:
         break;
      

      default:
         break;
   }
   this->_State = state;
   Serial.print("\n GarmentBot::SetMode()" );
   Serial.println(state);

   
}

// return true:   parked
// return false:  still parking
bool GarmentAgv::DoParking(){
    int track_error = this->trackSensor->ReadError_LeftRight();
    int position_error = this->trackSensor->ReadError_FrontRear();

    this->SetTargetSpeed(1);
    if (track_error < 10 && position_error < 10)
        return true;
    return false;
}