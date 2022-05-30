#include "all_applications.h"
// #include "all_devices.h"
#ifdef I_AM_GARMENT_BOT
#include "garment_bot.h"

GarmentBot::GarmentBot(uint16_t id){
	this->_ID = id;
}

void GarmentBot::Init(){
	Serial.print("\n[Info] GarmentBot::Init() is entering");
	this->objRfid.Init(17,18,19);
	// this->objRfid.LinkCallback(&onDetectedMark);
	this->objAgv.Init();
	pinMode(PIN_BATTERY_VOLTAGE_ADC, INPUT);

	this->ToState(GarmentBot::BOT_STATE::BOT_LOCATING);
	Serial.print("\n[Info] GarmentBot::Init() is done.\n");
}

void GarmentBot::ExecuteMqttCommand(const char* command){
	// command examples
	// "99,short_cut" == leave main road.
	// "11,load"      == move to point 23, loading 
	// "22,unloads"   == move to point 25, unloading
	// "33,charge"
	// "44,sleep"
	int16_t node_id;
	String str_command;

	if (str_command == "short_cut"){

	} else if (str_command == "load"){
		this->objBoxMoverAgent.PresetState(GarmentBoxMoverAgent::BoxMoverState::LOADING);
	} else if (str_command == "unload"){
	} else if (str_command == "charge"){

	} else if (str_command == "sleep"){

	}
}

void GarmentBot::onDetectedMark(uint16_t BranchNode_id){
   RoadBranchNode current_BranchNode;
   if (this->objMapNavigator.FetchNode(BranchNode_id, &current_BranchNode)){
         // the mark is being managered via map navigator.
	switch (current_BranchNode.task){
	case RoadBranchNode::TASK::SHORT_CUT_ONLY:
		// Follow branch road, not main road.
		this->objAgv.SetFollowMainRoad(this->objRfid.MainRoad_IsOn_LeftSide , false);
		break;
	case RoadBranchNode::TASK::LOAD:
	  	// ???  This is invoked when agv is SLOW_MOVING, should to loading, after parking.
         this->__current_BranchNode.task = RoadBranchNode::TASK::LOAD;
         this->ToState(GarmentBot::BOT_STATE::AGV_PARKED_AT_SOURCE);
         break;
	case RoadBranchNode::TASK::UNLOAD:
         this->__current_BranchNode.task = RoadBranchNode::TASK::UNLOAD;
         this->ToState(GarmentBot::BOT_STATE::AGV_PARKED_AT_SOURCE);
         break;
	case RoadBranchNode::TASK::SLEEP:
         this->__current_BranchNode.task = RoadBranchNode::TASK::SLEEP;
         this->ToState(GarmentBot::AGV_PARKED_AT_SOURCE);
         break;
	case RoadBranchNode::TASK::CHARGE:
         this->__current_BranchNode.task = RoadBranchNode::TASK::CHARGE;
         this->ToState(GarmentBot::AGV_PARKED_AT_SOURCE);
		 break;         
	default:
         break;
      }
   }
}

void GarmentBot::SpinOnce(){
	uint16_t battery_voltage =  analogRead(PIN_BATTERY_VOLTAGE_ADC) ;
	this->__battery_voltage = 1.0 * battery_voltage + 0.0;
	
	this->objBoxMoverAgent.SpinOnce();
	this->objAgv.SpinOnce();
	this->CheckMqttCommand();

	
	if(this->objAgv.GetState() == AgvBase::AGV_STATE::SLOW_MOVING ){
		if (this->objRfid.ReadCard()){
			this->onDetectedMark(this->objRfid.CardId);
			return;
		}
	}

	switch (this->__state){
	case GarmentBot::BOT_STATE::BOT_LOCATING:
		//Trying to read RFID.
		// if (this->__rfidReader->PICC_ReadCardSerial() == 123){
		if (this->objRfid.CardId == 123){
			this->ToState(GarmentBot::BOT_STATE::BOT_SLEEPING);
		}
		break;
   	case GarmentBot::BOT_STATE::BOT_CHARGING:
      	break;
   	case GarmentBot::BOT_STATE::BOT_SLEEPING:
      	break;
   	case GarmentBot::BOT_STATE::BOT_EMERGENCY_STOPING:
      	break;
   	case GarmentBot::BOT_STATE::AGV_MOVING_TO_SOURCE:
		if(this->objAgv.GetState() == AgvBase::AGV_STATE::PARKED)
			this->ToState(GarmentBot::BOT_STATE::AGV_PARKED_AT_SOURCE);

		break;
	case GarmentBot::BOT_STATE::ROBOT_LOADING:
		if(this->objBoxMoverAgent.ReadState() == GarmentBoxMoverAgent::BoxMoverState::LOADED)
			this->ToState(GarmentBot::BOT_STATE::AGV_MOVING_TO_DESTINATION);
		break;
	case GarmentBot::BOT_STATE::AGV_MOVING_TO_DESTINATION:
		if(this->objAgv.GetState() == AgvBase::AGV_STATE::PARKED)
			this->ToState(GarmentBot::BOT_STATE::AGV_PARKED_AT_DESTINATION);
		break;
	case GarmentBot::BOT_STATE::AGV_PARKED_AT_DESTINATION:
		if(this->objBoxMoverAgent.ReadState() == GarmentBoxMoverAgent::BoxMoverState::UNLOADED)
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
	case GarmentBot::BOT_STATE::BOT_LOCATING:
		this->objAgv.ToState(AgvBase::AGV_STATE::SLOW_MOVING);
		break;
	case GarmentBot::BOT_STATE::BOT_SLEEPING:
		// Keep reporting battery voltage.
		this->objAgv.ToState(AgvBase::AGV_STATE::PARKED);
		break;
	case GarmentBot::BOT_STATE::AGV_MOVING_TO_SOURCE:
		this->objAgv.ToState(AgvBase::AGV_STATE::FAST_MOVING);
		break;
	case GarmentBot::BOT_STATE::AGV_PARKED_AT_SOURCE:
		this->objBoxMoverAgent.ToPresetState();
		new_state = GarmentBot::BOT_STATE::ROBOT_LOADING;
		break;
	case GarmentBot::BOT_STATE::AGV_MOVING_TO_DESTINATION:
		this->objAgv.ToState(AgvBase::AGV_STATE::FAST_MOVING);
		break;
	case GarmentBot::BOT_STATE::AGV_PARKED_AT_DESTINATION:
		this->objBoxMoverAgent.ToPresetState();
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
   	if (test_id == 1) {
		this->objBoxMoverAgent.PresetState(GarmentBoxMoverAgent::BoxMoverState::LOADING);
		this->objBoxMoverAgent.ToPresetState();
	   }
   if (test_id==10) {
        // int track_error = 0;
      //   this->objTwinWheelHardware.MoveForward(track_error);
   }
}

uint8_t GarmentBot::GetMqtt_PubPayload(uint8_t* chars){
    // a json string,, constructed by state,battery_volt,last_site_id
    String payload = "{\"id\":";
	payload.concat(String(this->_ID));
    payload.concat(",\"sta\":");
    payload.concat(String(this->__state));
    payload.concat(",\"bat\":");
    payload.concat(String(12.8));
    payload.concat(",\"nd\":");
    payload.concat(String(45));
	payload.concat("} ");   //Space is important !! for length =+1
    // uint8_t length = payload.length();
    // uint8_t* dest = (uint8_t *)chars;
	bool debug = false;
	if (debug){
		Serial.print("[Debug] GarmentBot::GetMqtt_PubPayload()    length= ");
		Serial.println(payload.length());
	}
    payload.getBytes(chars, payload.length());
    return payload.length();
}

// void GarmentBot::onMqttReceived(uint8_t* payload){
//    // Currently is for testing, 
//    // Normally this function will be callback via a MQTT client.
// //    this->objMapNavigator.AddNode(1, BranchNode::TASK::FOLLOW_LEFT);
//    this->objMapNavigator.AddNode(2, RoadBranchNode::TASK::LOAD);
//    this->objMapNavigator.AddNode(3, RoadBranchNode::TASK::UNLOAD);
//    this->objMapNavigator.AddNode(4, RoadBranchNode::TASK::SLEEP);
// }

#endif