#include "garment_agv.h"


GarmentAgv::GarmentAgv(){
    this->trackSensor = new TrackSensor_Dual9960(1,2,3,4);
    // this->sensor_left_wheel = new HallSensor(2, 3, 4, 11);
    // this->sensor_left_wheel.pullup = Pullup::USE_INTERN;
    // this->leftWheel.sen
} 

// void GarmentAgv::LinkTrackSensor(AgvTrackSensor* tracksensor){
//     this->trackSensor = tracksensor;
// }


void GarmentAgv::SpinOnce(){
    // int distance_to_full_park = 100;      //???
    // bool loading_finished = true;         // From mqtt
    // bool unloading_finished = true;       // from mqtt
    // uint16_t track_node_id = 0;               // read from RFID

    // Obstacle detection
    this->found_obstacle = false;
    // float distance =  this->obstacleSensor->measureDistanceCm(); 
    float distance_to_obstacle =  22; 
    if (distance_to_obstacle >0 && distance_to_obstacle <50) 
        this->found_obstacle = true;


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
            x_error = this->trackSensor->SpinOnce_Forwarding();
            this->MoveForward(x_error);
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
        // if (this->__current_navigator_point.LOADING)
        //     this->ToState(LOADING);
        // else if (this->__current_navigator_point.UNLOADING)
        //     this->ToState(UNLOADING);
        // else if (this->__current_navigator_point.SLEEPING)
        //     this->ToState(SLEEPING);
        // else if (this->__current_navigator_point.CHARGING)
        //     this->ToState(CHARGING);
        break;
//    case LOADING:
//         if (loading_finished)
//             this->ToState(FAST_MOVING);
//         break;
//    case UNLOADING:
//       if (unloading_finished)
//          this->ToState(FAST_MOVING);
//       break;
//    case SLEEPING:
//       /* code */
//       break;
   
   default:
      break;
   }
}

void GarmentAgv::ToState(AGV_STATE state){
   if (state == this->_State) return;
   switch(state){
    //   case SLEEPING:
    //         this->Stop();
    //         break;
      case FAST_MOVING:
            this->SetForwardSpeed(220);
            break;
      case SLOW_MOVING:
            this->SetForwardSpeed(100);
            break;
      case PARKING:
            this->SetForwardSpeed(20);
            break;
    //   case LOADING:
    //         // this->LoadBox();
    //         break;
    //   case UNLOADING:
    //         // this->objBoxMover.UnloadBox();
    //         break;
    //   case CHARGING:
    //      break;
      

      default:
         break;
   }
   this->_State = state;
   Serial.print("\n GarmentBot::SetMode()" );
   Serial.println(state);

   
}

// return true:   reach parked position
// return false:  still parking
bool GarmentAgv::DoParking(){
    int16_t x_error ;
    int16_t y_error ;
    this->trackSensor->SpinOnce_Parking(&x_error, &y_error);

    this->SetForwardSpeed(1);
    if (x_error < 10 && y_error < 10)
        return true;
    return false;
}