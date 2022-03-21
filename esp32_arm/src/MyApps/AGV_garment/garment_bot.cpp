#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT
#include "garment_bot.h"

GarmentBot::GarmentBot(){
}

void GarmentBot::Init(){
   Serial.print("\n[Info] GarmentBot::Init() is entering");
   // Setting PWM channels properties, Totally Esp32 has 16 channels
   const int freq = 30000;
   const int resolution = 8;   // so max pwm speed is 255
   ledcSetup(PWM_CHANNEL_0, freq, resolution); // configure LED PWM functionalitites
   ledcSetup(PWM_CHANNEL_1, freq, resolution); 

   // Init I2C bus
   Wire.begin();
   this->objTwinWheelHardware.Init();
   // this->objBoxMover.ParkArms(true);

   this->_last_state = GarmentAgv::PARKING;
   this->ToState(GarmentAgv::SLEEPING);
   Serial.print("\n[Info] GarmentBot::Init() is done.");

}

void ReadI2C(){
   		// Moving follow the track.
		// Read I2C sensor, and obstacle 
		uint8_t slave_address = 0x3f;
		uint8_t RxBuffer[1];
		uint8_t n_bytes = 2;
		Wire.beginTransmission(slave_address);
		Wire.endTransmission(false);
		Wire.requestFrom(slave_address, n_bytes);    // request data from slave device
		int i=0;
		while (Wire.available() > 0) {  // slave may send less than requested
            uint8_t c = Wire.read();         // receive a byte as character
            RxBuffer[0] = c;
            i++;
            // Serial.println(c,BIN);
		}
		Wire.endTransmission(true);

		if (RxBuffer[1] > 0){
			// Got an obstacle, agv should be stop
			// this->objTwinWheel.Stop();s
		}
}


void GarmentBot::onDetectedMark(uint16_t BranchNode_id){
   BranchNode current_BranchNode;
   if (this->objMapNavigator.FetchNode(BranchNode_id, &current_BranchNode)){
         // the mark is being managered via map navigator.
      switch (current_BranchNode.task)
      {
      case BranchNode::TASK::FOLLOW_LEFT:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = false;
         this->ToState(GarmentAgv::FAST_MOVING);
         break;
      case BranchNode::TASK::FOLLLOW_RIGHT:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->ToState(GarmentAgv::FAST_MOVING);
         break;
      case BranchNode::TASK::LOADING:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_BranchNode.task = BranchNode::TASK::LOADING;
         this->ToState(GarmentAgv::PARKING);
         break;
      case BranchNode::TASK::UNLOADING:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_BranchNode.task = BranchNode::TASK::UNLOADING;
         this->ToState(GarmentAgv::PARKING);
         break;
      case BranchNode::TASK::SLEEPING:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_BranchNode.task = BranchNode::TASK::SLEEPING;
         this->ToState(GarmentAgv::PARKING);
         break;
      case BranchNode::TASK::CHARGING:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_BranchNode.task = BranchNode::TASK::CHARGING;
         this->ToState(GarmentAgv::PARKING);         
      default:
         break;
      }
   }
}

void GarmentBot::SpinOnce(){
   int distance_to_full_park = 100;
   bool loading_finished = true;
   bool unloading_finished = true;
   uint16_t BranchNode_id = 0;
   bool found_obstacle = false;
   bool found_slowdown_mark = false;
   int track_error = 0; // this->objTrackSensor.ReadError_FromRight(&RxBuffer[0]);
   int position_error = 100;

//    this->onMqttReceived();
   this->objBoxMover.SpinOnce();

   switch (this->_State)
   {
   case GarmentAgv::FAST_MOVING:
        if (found_obstacle)
            this->ToState(GarmentAgv::FAST_MOVING_PAUSED);
        // check if see the mark of slow-down.
        else if (found_slowdown_mark)
            this->ToState(GarmentAgv::SLOW_MOVING);
        else
            this->objTwinWheelHardware.MoveForward(track_error);
        break;
    case GarmentAgv::FAST_MOVING_PAUSED:
        if (!found_obstacle)
            this->ToState(GarmentAgv::FAST_MOVING);
        break;
    case GarmentAgv::SLOW_MOVING:
        if (found_obstacle)
            this->ToState(GarmentAgv::SLOW_MOVING_PAUSED);
        else {
            //try to read RFID
            BranchNode_id = this->objRemoteSensor.ObjRfidReader.last_card_id; 
            if (BranchNode_id > 0){
                // got the mark
                this->onDetectedMark(BranchNode_id);
            }
        }
        break;
   case GarmentAgv::SLOW_MOVING_PAUSED:
      if (!found_obstacle)
         this->ToState(GarmentAgv::SLOW_MOVING);
      break;
   case GarmentAgv::PARKING:
        // try to finish parking
        if (distance_to_full_park == 0)
            this->ToState(GarmentAgv::PARKED);
        else{
            // position_error = this->objPositionSensor.ReadError_FromRight();
            this->objTwinWheelHardware.SetTargetSpeed(position_error);
        }
        break;
   case GarmentAgv::PARKING_PAUSED:
        if (!found_obstacle)
            this->ToState(GarmentAgv::PARKING);
            break;
   case GarmentAgv::PARKED:
        if (this->__current_BranchNode.LOADING)
            this->ToState(GarmentAgv::LOADING);
        else if (this->__current_BranchNode.UNLOADING)
            this->ToState(GarmentAgv::UNLOADING);
        else if (this->__current_BranchNode.SLEEPING)
            this->ToState(GarmentAgv::SLEEPING);
        else if (this->__current_BranchNode.CHARGING)
            this->ToState(GarmentAgv::CHARGING);
        break;
   case GarmentAgv::LOADING:
        if (loading_finished)
            this->ToState(GarmentAgv::FAST_MOVING);
        break;
   case GarmentAgv::UNLOADING:
      if (unloading_finished)
         this->ToState(GarmentAgv::FAST_MOVING);
      break;
   case GarmentAgv::SLEEPING:
      /* code */
      break;
   
   default:
      break;
   }
}

void GarmentBot::ToState(GarmentAgv::GARMENTAGV_STATE state){
   if (state == this->_State) return;
   switch(state){
      case GarmentAgv::SLEEPING:
            this->objTwinWheelHardware.Stop();
            break;
      case GarmentAgv::FAST_MOVING:
            this->objTwinWheelHardware.SetTargetSpeed(220);
            break;
      case GarmentAgv::SLOW_MOVING:
            this->objTwinWheelHardware.SetTargetSpeed(100);
            break;
      case GarmentAgv::PARKING:
            this->objTwinWheelHardware.SetTargetSpeed(20);
            break;
      case GarmentAgv::LOADING:
            this->objBoxMover.LoadBox();
            break;
      case GarmentAgv::UNLOADING:
            this->objBoxMover.UnloadBox();
            break;
      case GarmentAgv::CHARGING:
         break;
      

      default:
         break;
   }
   this->_State = state;
   Serial.print("\n GarmentBot::SetMode()" );
   Serial.println(state);

   
}

void GarmentBot::Test(int test_id){
   if (test_id == 1) this->objBoxMover.LoadBox();
   if (test_id == 2) this->objBoxMover.UnloadBox();
   if (test_id==10) {
        int track_error = 0;
        this->objTwinWheelHardware.MoveForward(track_error);
   }
}

uint8_t GarmentBot::GetMqtt_PubPayload(uint8_t* chars){
    // a json string,, constructed by state,battery_volt,last_site_id
    String payload = "agv_id:" + this->_ID;
    payload.concat(",state:");
    payload.concat(this->_State);
    payload.concat(",battery:");
    payload.concat(123);
    payload.concat(",site:");
    payload.concat(this->_last_state);
    // uint8_t length = payload.length();
    // uint8_t* dest = (uint8_t *)chars;
    payload.getBytes(chars, payload.length());
    return payload.length();
}

//void GarmentBot::CommuWithUppper(){
void GarmentBot::onMqttReceived(uint8_t* payload){
   // Currently is for testing, 
   // Normally this function will be callback via a MQTT client.
   this->objMapNavigator.AddNode(1, BranchNode::TASK::FOLLOW_LEFT);
   this->objMapNavigator.AddNode(2, BranchNode::TASK::LOADING);
   this->objMapNavigator.AddNode(3, BranchNode::TASK::UNLOADING);
   this->objMapNavigator.AddNode(4, BranchNode::TASK::SLEEPING);

}

#endif