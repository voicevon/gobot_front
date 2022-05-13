#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT_SINGLE_MCU
#include "bot_single_mcu.h"

#define PIN_IR_FRONT  35
#define PIN_IR_REAR  36

BotSingleMcu::BotSingleMcu(uint16_t id){
	this->_ID = id;
	// this->objBoxCarrier_hardware = new BoxCarrierHardware();
}

void BotSingleMcu::Init(){
	Serial.print("\n[Info] BotSingleMcu::Init() is entering");
	this->objRfid.Init(17,18,19);
	// this->objRfid.LinkCallback(&onDetectedMark);
	this->objAgv.Init();
	pinMode(PIN_BATTERY_VOLTAGE_ADC, INPUT);

	this->ToState(BotSingleMcu::BOT_STATE::BOT_LOCATING);
	Serial.print("\n[Info] BotSingleMcu::Init() is done.\n");
	
	this->_gcode_queue = new GcodeQueue();
	BoxCarrierHardware* objBoxCarrierHardware = new BoxCarrierHardware();
    objBoxCarrierHardware->InitRobot();
    objBoxCarrierHardware->LinkLocalGcodeQueue_AsConsumer(this->_gcode_queue);

	// #define I2C1_SDA_PIN 1
	// #define I2C1_SCL_PIN 3	
	// #define I2C2_SDA_PIN 1
	// #define I2C2_SCL_PIN 3
	// initialize ADS1115 on I2C bus 1 with default address 0x48
	// TwoWire* i2c_bus_a;
	// TwoWire* i2c_bus_b;
	// i2c_bus_a = new TwoWire(1);
	// i2c_bus_b = new TwoWire(1);
	// i2c_bus_a->begin(I2C1_SDA_PIN, I2C1_SCL_PIN, 100000); // Start I2C1 on pins 21 and 22
    // i2c_bus_b->begin(I2C2_SDA_PIN, I2C2_SCL_PIN, 100000); // Start I2C2 on pins 0 and 23

	// this->objAds1115 = new ADS1115(0x48, i2c_bus_a);
	// if (!this->objAds1115->isConnected()) {
	// 	// error ADS1115 not connected
	// }
}

void BotSingleMcu::ExecuteMqttCommand(const char* command){
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

void BotSingleMcu::onDetectedMark(uint16_t BranchNode_id){
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
		this->ToState(BotSingleMcu::BOT_STATE::AGV_PARKED_AT_SOURCE);
		break;
	case RoadBranchNode::TASK::UNLOAD:
		this->__current_BranchNode.task = RoadBranchNode::TASK::UNLOAD;
		this->ToState(BotSingleMcu::BOT_STATE::AGV_PARKED_AT_SOURCE);
		break;
	case RoadBranchNode::TASK::SLEEP:
		this->__current_BranchNode.task = RoadBranchNode::TASK::SLEEP;
		this->ToState(BotSingleMcu::AGV_PARKED_AT_SOURCE);
		break;
	case RoadBranchNode::TASK::CHARGE:
		this->__current_BranchNode.task = RoadBranchNode::TASK::CHARGE;
		this->ToState(BotSingleMcu::AGV_PARKED_AT_SOURCE);
		break;         
	default:
		break;
      }
   }
}

int BotSingleMcu::ReadAlignment_sensors(){
	// Read 4-channels adc from ADS1115
	// this->Alignment_value_top = this->objAds1115->readADC(0);
	// this->Alignment_value_bottom = this->objAds1115->readADC(1);
	// this->Alignment_value_front = this->objAds1115->readADC(2);
	// this->Alignment_value_rear = this->objAds1115->readADC(3);
	int front = analogRead(PIN_IR_FRONT);
	int rear = analogRead(PIN_IR_REAR);
	return front-rear;
}

void BotSingleMcu::SpinOnce(){
	String gcode = "G1";
	int align_error=0;
	uint16_t battery_voltage =  analogRead(PIN_BATTERY_VOLTAGE_ADC) ;
	this->__battery_voltage = 1.0 * battery_voltage + 0.0;
	// this->objBoxMoverAgent.SpinOnce();
	// this->objBoxCarrier_hardware->SpinOnce();
	this->objAgv.SpinOnce();
	this->objBoxCarrierHardware->SpinOnce();
	this->objBoxCarrier.SpinOnce();
	this->CheckMqttCommand();

	
	if(this->objAgv.GetState() == TwinWheelsAgv::AGV_STATE::SLOW_MOVING ){
		if (this->objRfid.ReadCard()){
			this->onDetectedMark(this->objRfid.CardId);
			return;
		}
	}

	switch (this->__state){
	case BotSingleMcu::BOT_STATE::BOT_LOCATING:
		//Trying to read RFID.
		// if (this->__rfidReader->PICC_ReadCardSerial() == 123){
		if (this->objRfid.CardId == 123){
			this->ToState(BotSingleMcu::BOT_STATE::BOT_SLEEPING);
		}
		break;
   	case BotSingleMcu::BOT_STATE::BOT_CHARGING:
      	break;
   	case BotSingleMcu::BOT_STATE::BOT_SLEEPING:
      	break;
   	case BotSingleMcu::BOT_STATE::BOT_EMERGENCY_STOPING:
      	break;
   	case BotSingleMcu::BOT_STATE::AGV_MOVING_TO_SOURCE:
		if(this->objAgv.GetState() == TwinWheelsAgv::AGV_STATE::PARKED)
			this->ToState(BotSingleMcu::BOT_STATE::AGV_PARKED_AT_SOURCE);
		break;
	case BotSingleMcu::BOT_STATE::AGV_PARKED_AT_SOURCE:
		this->objBoxCarrier.LoadBox();
		this->ToState(BotSingleMcu::BOT_STATE::ROBOT_LOAD_ALIGN);

		break;
	case BotSingleMcu::BOT_STATE::ROBOT_LOAD_ALIGN:
		if (this->objBoxCarrierHardware->State == RobotState::IDLE){
			// Last movement is done.
			align_error = this->ReadAlignment_sensors();
			if (align_error < 100){
				this->ToState(BotSingleMcu::BOT_STATE::ROBOT_LOADING);
			}else{
				gcode.concat("Y");
				gcode.concat(align_error);
				this->_gcode_queue->AppendGcodeCommand(gcode);
			}
		}
		break;
	case BotSingleMcu::BOT_STATE::ROBOT_LOADING:
		// if(this->objBoxMoverAgent.ReadState() == GarmentBoxMoverAgent::BoxMoverState::LOADED)
		if (this->objBoxCarrier.GetState()== BoxCarrier::BoxMoverState::LOADED)
			this->ToState(BotSingleMcu::BOT_STATE::AGV_MOVING_TO_DESTINATION);
		break;
	case BotSingleMcu::BOT_STATE::AGV_MOVING_TO_DESTINATION:
		if(this->objAgv.GetState() == TwinWheelsAgv::AGV_STATE::PARKED)
			this->ToState(BotSingleMcu::BOT_STATE::AGV_PARKED_AT_DESTINATION);
		break;
	case BotSingleMcu::BOT_STATE::AGV_PARKED_AT_DESTINATION:
		// if(this->objBoxMoverAgent.ReadState() == GarmentBoxMoverAgent::BoxMoverState::UNLOADED)
			// Check battery voltage.
			this->ToState(BotSingleMcu::BOT_STATE::BOT_SLEEPING);
		break;
   default:
		Serial.print("\n [Warning] BotSingleMcu::SpinOnce()  A state with out dealer.! \n\n");
      	break;
   }
}

void BotSingleMcu::ToState(BotSingleMcu::BOT_STATE state){
	String gcode ="G1";

	if (state == this->__state) {
		Serial.print("[Warning] BotSingleMcu::ToState()  Repeated! \n\n");
		return;
	}
 	BotSingleMcu::BOT_STATE new_state = state;
	switch(state){
	case BotSingleMcu::BOT_STATE::BOT_LOCATING:
		this->objAgv.ToState(TwinWheelsAgv::AGV_STATE::SLOW_MOVING);
		break;
	case BotSingleMcu::BOT_STATE::BOT_SLEEPING:
		// Keep reporting battery voltage.
		this->objAgv.ToState(TwinWheelsAgv::AGV_STATE::PARKED);
		break;
	case BotSingleMcu::BOT_STATE::AGV_MOVING_TO_SOURCE:
		this->objAgv.ToState(TwinWheelsAgv::AGV_STATE::FAST_MOVING);
		break;
	case BotSingleMcu::BOT_STATE::AGV_PARKED_AT_SOURCE:
		// this->objBoxMoverAgent.ToPresetState();
		// this->obj
		new_state = BotSingleMcu::BOT_STATE::ROBOT_LOAD_ALIGN;
		break;
	case BotSingleMcu::BOT_STATE::ROBOT_LOAD_ALIGN:
		if (this->objBoxCarrierHardware->State == RobotState::IDLE){
			gcode.concat("Z50");
			this->_gcode_queue->AppendGcodeCommand(gcode);
			new_state = BotSingleMcu::BOT_STATE::ROBOT_LOADING;
		}
		break;
	case BotSingleMcu::BOT_STATE::ROBOT_LOADING:
		if (this->objBoxCarrierHardware->State == RobotState::IDLE){
			new_state = BotSingleMcu::BOT_STATE::AGV_MOVING_TO_DESTINATION;
		}
		break;
	case BotSingleMcu::BOT_STATE::AGV_MOVING_TO_DESTINATION:
		this->objAgv.ToState(TwinWheelsAgv::AGV_STATE::FAST_MOVING);
		break;
	case BotSingleMcu::BOT_STATE::AGV_PARKED_AT_DESTINATION:
		// this->objBoxMoverAgent.ToPresetState();
		new_state = BotSingleMcu::BOT_STATE::ROBOT_UNLOADING;
		break;

	case BotSingleMcu::BOT_STATE::ROBOT_UNLOADING:
		break;

	default:
		Serial.print("\n\n [Warn]  BotSingleMcu::ToState()   Case Else...");
		break;
   }
   this->__state = new_state;
}

void BotSingleMcu::Test(int test_id){
   	if (test_id == 1) {
		// this->objBoxMoverAgent.PresetState(GarmentBoxMoverAgent::BoxMoverState::LOADING);
		// this->objBoxMoverAgent.ToPresetState();
	   }
   if (test_id==10) {
        // int track_error = 0;
        // this->objTwinWheelHardware.MoveForward(track_error);
		// this->objAgv.
   }
}

uint8_t BotSingleMcu::GetMqtt_PubPayload(uint8_t* chars){
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
		Serial.print("[Debug] BotSingleMcu::GetMqtt_PubPayload()    length= ");
		Serial.println(payload.length());
	}
    payload.getBytes(chars, payload.length());
    return payload.length();
}

// void BotSingleMcu::onMqttReceived(uint8_t* payload){
//    // Currently is for testing, 
//    // Normally this function will be callback via a MQTT client.
// //    this->objMapNavigator.AddNode(1, BranchNode::TASK::FOLLOW_LEFT);
//    this->objMapNavigator.AddNode(2, RoadBranchNode::TASK::LOAD);
//    this->objMapNavigator.AddNode(3, RoadBranchNode::TASK::UNLOAD);
//    this->objMapNavigator.AddNode(4, RoadBranchNode::TASK::SLEEP);
// }

#endif