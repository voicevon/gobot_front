#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT
#include "garment_bot.h"

GarmentBot::GarmentBot(){

}

void GarmentBot::Init(){
	Serial.print("\n[Info] GarmentBot::Init() is entering");
	this->rfidReader = new MFRC522(17,18);
	this->objAgv.Init();

	// Init I2C bus
	Wire.begin();

	this->ToState(GarmentBot::BOT_STATE::BOT_SLEEPING);
	Serial.print("\n[Info] GarmentBot::Init() is done.\n");
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
         // this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = false;
         this->ToState(GarmentBot::BOT_STATE::AGV_MOVING_TO_SOURCE);
         break;
      case BranchNode::TASK::FOLLLOW_RIGHT:
         // this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->ToState(GarmentBot::BOT_STATE::AGV_MOVING_TO_SOURCE);
         break;
      case BranchNode::TASK::LOADING:
         // this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_BranchNode.task = BranchNode::TASK::LOADING;
         this->ToState(GarmentBot::BOT_STATE::AGV_PARKED_AT_SOURCE);
         break;
      case BranchNode::TASK::UNLOADING:
         // this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_BranchNode.task = BranchNode::TASK::UNLOADING;
         this->ToState(GarmentBot::BOT_STATE::AGV_PARKED_AT_SOURCE);
         break;
      case BranchNode::TASK::SLEEPING:
         // this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_BranchNode.task = BranchNode::TASK::SLEEPING;
         this->ToState(GarmentBot::AGV_PARKED_AT_SOURCE);
         break;
      case BranchNode::TASK::CHARGING:
         // this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_BranchNode.task = BranchNode::TASK::CHARGING;
         this->ToState(GarmentBot::AGV_PARKED_AT_SOURCE);         
      default:
         break;
      }
   }
}

void GarmentBot::SpinOnce(){
//    int distance_to_full_park = 100;
//    bool loading_finished = true;
//    bool unloading_finished = true;
//    uint16_t BranchNode_id = 0;
//    bool found_obstacle = false;
//    bool found_slowdown_mark = false;
//    // int track_error = 0; // this->objTrackSensor.ReadError_FromRight(&RxBuffer[0]);
//    int position_error = 100;

//    this->onMqttReceived();
   this->objBoxMover.SpinOnce();
   this->objAgv.SpinOnce();

   switch (this->__state)
   	{
   	case GarmentBot::BOT_STATE::BOT_CHARGING:
      	break;
   	case GarmentBot::BOT_STATE::BOT_SLEEPING:
      	break;
   	case GarmentBot::BOT_STATE::BOT_EMERGENCY_STOPING:
      	break;
   	case GarmentBot::BOT_STATE::AGV_MOVING_TO_SOURCE:
		if(this->objAgv.GetState() == TwinWheelsAgv::AGV_STATE::PARKED)
			this->ToState(GarmentBot::BOT_STATE::AGV_PARKED_AT_SOURCE);
		break;
	case GarmentBot::BOT_STATE::ROBOT_LOADING:
		if(this->objBoxMover.GetState() == BoxMover::BoxMoverState::LOADED)
			this->ToState(GarmentBot::BOT_STATE::AGV_MOVING_TO_DESTINATION);
		break;
	case GarmentBot::BOT_STATE::AGV_MOVING_TO_DESTINATION:
		if(this->objAgv.GetState() == TwinWheelsAgv::AGV_STATE::PARKED)
			this->ToState(GarmentBot::BOT_STATE::AGV_PARKED_AT_DESTINATION);
		break;
	case GarmentBot::BOT_STATE::AGV_PARKED_AT_DESTINATION:
		if(this->objBoxMover.GetState() == BoxMover::BoxMoverState::UNLOADED)
			// Check battery voltage.
			this->ToState(GarmentBot::BOT_STATE::BOT_SLEEPING);
		break;
   default:
		Serial.print("\n [Warning] GarmentBot::SpinOnce()  A state with out dealer.! \n\n");
      	break;
   }
}

void GarmentBot::ToState(GarmentBot::BOT_STATE state){
	if (state == this->__state) {
		Serial.print("[Warning] GarmentBot::ToState()  Repeated! \n\n");
		return;
	}
 	GarmentBot::BOT_STATE new_state = state;
	switch(state){
	case GarmentBot::BOT_STATE::BOT_SLEEPING:
		// Keep reporting battery voltage.
		this->objAgv.ToState(TwinWheelsAgv::AGV_STATE::PARKED);
		break;
	case GarmentBot::BOT_STATE::AGV_MOVING_TO_SOURCE:
		this->objAgv.ToState(TwinWheelsAgv::AGV_STATE::FAST_MOVING);
		break;
	case GarmentBot::BOT_STATE::AGV_PARKED_AT_SOURCE:
		this->objBoxMover.LoadBox();
		new_state = GarmentBot::BOT_STATE::ROBOT_LOADING;
		break;
	case GarmentBot::BOT_STATE::AGV_MOVING_TO_DESTINATION:
		this->objAgv.ToState(TwinWheelsAgv::AGV_STATE::FAST_MOVING);
		break;
	case GarmentBot::BOT_STATE::AGV_PARKED_AT_DESTINATION:
		this->objBoxMover.UnloadBox();
		new_state = GarmentBot::BOT_STATE::ROBOT_UNLOADING;
		break;
	
	case GarmentBot::BOT_STATE::ROBOT_LOADING:
		break;
	case GarmentBot::BOT_STATE::ROBOT_UNLOADING:
		break;

	default:
		Serial.print("\n\n [Warn]  GarmentBot::ToState()   Case Else...");
		break;
   }
   this->__state = new_state;
}

void GarmentBot::Test(int test_id){
   if (test_id == 1) this->objBoxMover.LoadBox();
   if (test_id == 2) this->objBoxMover.UnloadBox();
   if (test_id==10) {
        // int track_error = 0;
      //   this->objTwinWheelHardware.MoveForward(track_error);
   }
}

uint8_t GarmentBot::GetMqtt_PubPayload(uint8_t* chars){
    // a json string,, constructed by state,battery_volt,last_site_id
    String payload = "agv_id:" + this->_ID;
    payload.concat(",state:");
    payload.concat(this->__state);
    payload.concat(",battery:");
    payload.concat(123);
    payload.concat(",site:");
    // payload.concat(this->__last_state);
    // uint8_t length = payload.length();
    // uint8_t* dest = (uint8_t *)chars;
    payload.getBytes(chars, payload.length());
    return payload.length();
}

void GarmentBot::onMqttReceived(uint8_t* payload){
   // Currently is for testing, 
   // Normally this function will be callback via a MQTT client.
   this->objMapNavigator.AddNode(1, BranchNode::TASK::FOLLOW_LEFT);
   this->objMapNavigator.AddNode(2, BranchNode::TASK::LOADING);
   this->objMapNavigator.AddNode(3, BranchNode::TASK::UNLOADING);
   this->objMapNavigator.AddNode(4, BranchNode::TASK::SLEEPING);

}

#endif