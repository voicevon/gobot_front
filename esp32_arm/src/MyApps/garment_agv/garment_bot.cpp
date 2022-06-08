#include "all_applications.h"
#ifdef I_AM_ROBOT_ASRS_AGV
#include "garment_bot.h"

BotAsrsAgvCoreYZ::BotAsrsAgvCoreYZ(uint16_t id){
	this->_ID = id;
}

void BotAsrsAgvCoreYZ::InitAllinOne(BoardAllInOne* board, CncMachineBase* cncMachine, StepControl* stepControl){
	Serial.print("\n[Info] BotAsrsAgvCoreYZ::Init() is entering");
	this->agv.Init(&board->agv);
	this->asrs.LinkJettySensor(board->asrs.GetJettySensor());
	this->cnc.Init(&board->cnc_board, cncMachine);
	this->cnc.LinkStepControl(stepControl);
	
    this->cnc.LinkLocalGcodeQueue_AsConsumer(&this->_gcode_queue);
	this->ToState(BotAsrsAgvCoreYZ::BOT_STATE::BOT_LOCATING);
	Serial.print("\n[Info] BotAsrsAgvCoreYZ::Init() is done.\n");
}

void BotAsrsAgvCoreYZ::ExecuteMqttCommand(const char* command){
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
		// this->objBoxMoverAgent.PresetState(GarmentBoxMoverAgent::BoxMoverState::LOADING);
	} else if (str_command == "unload"){
	} else if (str_command == "charge"){

	} else if (str_command == "sleep"){

	}
}

void BotAsrsAgvCoreYZ::onDetectedMark(uint16_t BranchNode_id){
   RoadBranchNode current_BranchNode;
   if (this->objMapNavigator.FetchNode(BranchNode_id, &current_BranchNode)){
		// the mark is being managered via map navigator.
	switch (current_BranchNode.task){
	case RoadBranchNode::TASK::SHORT_CUT_ONLY:
		// Follow branch road, not main road.
		// this->agv.SetFollowMainRoad(this->objRfid.MainRoad_IsOn_LeftSide , false);
		this->agv.SetFollowMainRoad(this->agv.RfidReader->MainRoad_IsOn_LeftSide , false);
		break;
	case RoadBranchNode::TASK::LOAD:
	  	// ???  This is invoked when agv is SLOW_MOVING, should to loading, after parking.
		this->__current_BranchNode.task = RoadBranchNode::TASK::LOAD;
		this->ToState(BotAsrsAgvCoreYZ::BOT_STATE::AGV_PARKED_AT_SOURCE);
		break;
	case RoadBranchNode::TASK::UNLOAD:
		this->__current_BranchNode.task = RoadBranchNode::TASK::UNLOAD;
		this->ToState(BotAsrsAgvCoreYZ::BOT_STATE::AGV_PARKED_AT_SOURCE);
		break;
	case RoadBranchNode::TASK::SLEEP:
		this->__current_BranchNode.task = RoadBranchNode::TASK::SLEEP;
		this->ToState(BotAsrsAgvCoreYZ::AGV_PARKED_AT_SOURCE);
		break;
	case RoadBranchNode::TASK::CHARGE:
		this->__current_BranchNode.task = RoadBranchNode::TASK::CHARGE;
		this->ToState(BotAsrsAgvCoreYZ::AGV_PARKED_AT_SOURCE);
		break;         
	default:
		break;
      }
   }
}


void BotAsrsAgvCoreYZ::SpinOnce(){
	String gcode = "G1";
	int align_error=0;
	this->agv.SpinOnce();
	this->cnc.SpinOnce();
	return;
	this->objBoxCarrier.SpinOnce();   // something wrong inside ?
	this->CheckMqttCommand();  //??
	return;


	// if(this->agv.GetState() == AgvBase::AGV_STATE::SLOW_MOVING ){
	// 	if (this->objRfid.ReadCard()){
	// 		this->onDetectedMark(this->objRfid.CardId);
	// 		return;
	// 	}
	// }
		Serial.println("555555555555555555");
	Serial.println(this->__state);
	switch (this->__state){
	case BotAsrsAgvCoreYZ::BOT_STATE::BOT_LOCATING:
		//Trying to read RFID.
		// if (this->__rfidReader->PICC_ReadCardSerial() == 123){
		if (this->agv.RfidReader->CardId == 123){
			this->ToState(BotAsrsAgvCoreYZ::BOT_STATE::BOT_SLEEPING);
		}
		break;
   	case BotAsrsAgvCoreYZ::BOT_STATE::BOT_CHARGING:
      	break;
   	case BotAsrsAgvCoreYZ::BOT_STATE::BOT_SLEEPING:
      	break;
   	case BotAsrsAgvCoreYZ::BOT_STATE::BOT_EMERGENCY_STOPING:
      	break;
   	case BotAsrsAgvCoreYZ::BOT_STATE::AGV_MOVING_TO_SOURCE:
		if(this->agv.GetState() == AgvBase::AGV_STATE::PARKED)
			this->ToState(BotAsrsAgvCoreYZ::BOT_STATE::AGV_PARKED_AT_SOURCE);
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::AGV_PARKED_AT_SOURCE:
		this->objBoxCarrier.LoadBox();
		this->ToState(BotAsrsAgvCoreYZ::BOT_STATE::ROBOT_LOAD_ALIGN);

		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::ROBOT_LOAD_ALIGN:
		if (this->cnc.State == CncState::IDLE){
			// Last movement is done.
			align_error = this->asrs.jettySensor->ReadAlignmentError();
			if (align_error < 100){
				this->ToState(BotAsrsAgvCoreYZ::BOT_STATE::ROBOT_LOADING);
			}else{
				gcode.concat("Y");
				gcode.concat(align_error);
				this->_gcode_queue.AppendGcodeCommand(gcode);
			}
		}
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::ROBOT_LOADING:
		// if(this->objBoxMoverAgent.ReadState() == GarmentBoxMoverAgent::BoxMoverState::LOADED)
		if (this->objBoxCarrier.GetState()== BoxCarrier::BoxMoverState::LOADED)
			this->ToState(BotAsrsAgvCoreYZ::BOT_STATE::AGV_MOVING_TO_DESTINATION);
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::AGV_MOVING_TO_DESTINATION:
		if(this->agv.GetState() == AgvBase::AGV_STATE::PARKED)
			this->ToState(BotAsrsAgvCoreYZ::BOT_STATE::AGV_PARKED_AT_DESTINATION);
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::AGV_PARKED_AT_DESTINATION:
		// if(this->objBoxMoverAgent.ReadState() == GarmentBoxMoverAgent::BoxMoverState::UNLOADED)
			// Check battery voltage.
			this->ToState(BotAsrsAgvCoreYZ::BOT_STATE::BOT_SLEEPING);
		break;
   default:
		Serial.print("\n [Warning] BotAsrsAgvCoreYZ::SpinOnce()  A state with out dealer.! \n\n");
      	break;
   }
		Serial.println("66666666666666666666666");
}

void BotAsrsAgvCoreYZ::ToState(BotAsrsAgvCoreYZ::BOT_STATE state){
	String gcode ="G1";

	if (state == this->__state) {
		Serial.print("[Warning] BotAsrsAgvCoreYZ::ToState()  Repeated! \n\n");
		return;
	}
 	BotAsrsAgvCoreYZ::BOT_STATE new_state = state;
	switch(state){
	case BotAsrsAgvCoreYZ::BOT_STATE::BOT_LOCATING:
		this->agv.ToState(AgvBase::AGV_STATE::SLOW_MOVING);
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::BOT_SLEEPING:
		// Keep reporting battery voltage.
		this->agv.ToState(AgvBase::AGV_STATE::PARKED);
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::AGV_MOVING_TO_SOURCE:
		this->agv.ToState(AgvBase::AGV_STATE::FAST_MOVING);
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::AGV_PARKED_AT_SOURCE:
		// this->objBoxMoverAgent.ToPresetState();
		// this->obj
		new_state = BotAsrsAgvCoreYZ::BOT_STATE::ROBOT_LOAD_ALIGN;
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::ROBOT_LOAD_ALIGN:
		if (this->cnc.State == CncState::IDLE){
			gcode.concat("Z50");
			this->_gcode_queue.AppendGcodeCommand(gcode);
			new_state = BotAsrsAgvCoreYZ::BOT_STATE::ROBOT_LOADING;
		}
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::ROBOT_LOADING:
		if (this->cnc.State == CncState::IDLE){
			new_state = BotAsrsAgvCoreYZ::BOT_STATE::AGV_MOVING_TO_DESTINATION;
		}
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::AGV_MOVING_TO_DESTINATION:
		this->agv.ToState(AgvBase::AGV_STATE::FAST_MOVING);
		break;
	case BotAsrsAgvCoreYZ::BOT_STATE::AGV_PARKED_AT_DESTINATION:
		// this->objBoxMoverAgent.ToPresetState();
		new_state = BotAsrsAgvCoreYZ::BOT_STATE::ROBOT_UNLOADING;
		break;

	case BotAsrsAgvCoreYZ::BOT_STATE::ROBOT_UNLOADING:
		break;

	default:
		Serial.print("\n\n [Warn]  BotAsrsAgvCoreYZ::ToState()   Case Else...");
		break;
   }
   this->__state = new_state;
}

// void BotAsrsAgvCoreYZ::Test(int test_id){
//    	if (test_id == 1) {
// 		// this->objBoxMoverAgent.PresetState(GarmentBoxMoverAgent::BoxMoverState::LOADING);
// 		// this->objBoxMoverAgent.ToPresetState();
// 	   }
//    if (test_id==10) {
//         // int track_error = 0;
//         // this->objTwinWheelHardware.MoveForward(track_error);
// 		// this->agv.
//    }
// }

void BotAsrsAgvCoreYZ::Test_HomeZ(){
	this->_gcode_queue.AppendGcodeCommand("G28Z");
}

void BotAsrsAgvCoreYZ::Test_HomeY(){
	this->_gcode_queue.AppendGcodeCommand("G28Y");
}

void BotAsrsAgvCoreYZ::Test_UnHome(){
	this->_gcode_queue.AppendGcodeCommand("G1Z1000Y300");
	this->_gcode_queue.AppendGcodeCommand("M84");
}

uint8_t BotAsrsAgvCoreYZ::GetMqtt_PubPayload(uint8_t* chars){
    // a json string,, constructed by state,battery_volt,last_site_id

	float battery_volt = this->agv.ReadBattery();
    String payload = "{\"id\":";
	payload.concat(String(this->_ID));
    payload.concat(",\"sta\":");
    payload.concat(String(this->__state));
    payload.concat(",\"bat\":");
    payload.concat(String(battery_volt));
    payload.concat(",\"nd\":");
    payload.concat(String(45));
	payload.concat("} ");   //Space is important !! for length =+1
    // uint8_t length = payload.length();
    // uint8_t* dest = (uint8_t *)chars;
	bool debug = false;
	if (debug){
		Serial.print("[Debug] BotAsrsAgvCoreYZ::GetMqtt_PubPayload()    length= ");
		Serial.println(payload.length());
		Serial.println(payload);
	}
    payload.getBytes(chars, payload.length());
    return payload.length();
}

// void BotAsrsAgvCoreYZ::onMqttReceived(uint8_t* payload){
//    // Currently is for testing, 
//    // Normally this function will be callback via a MQTT client.
// //    this->objMapNavigator.AddNode(1, BranchNode::TASK::FOLLOW_LEFT);
//    this->objMapNavigator.AddNode(2, RoadBranchNode::TASK::LOAD);
//    this->objMapNavigator.AddNode(3, RoadBranchNode::TASK::UNLOAD);
//    this->objMapNavigator.AddNode(4, RoadBranchNode::TASK::SLEEP);
// }

#endif