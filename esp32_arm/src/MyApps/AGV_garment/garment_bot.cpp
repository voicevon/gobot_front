#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT
#include "garment_bot.h"


void GarmentBot::Init(){
   // Setting PWM channels properties, Totally Esp32 has 16 channels
   Serial.print("\n[Info] GarmentBot::Init() is entering");
   const int freq = 30000;
   const int resolution = 8;   // so max pwm speed is 255
   ledcSetup(PWM_CHANNEL_0, freq, resolution); // configure LED PWM functionalitites
   ledcSetup(PWM_CHANNEL_1, freq, resolution); 
   // ledcSetup(PWM_CHANNEL_2, freq, resolution); 
   // ledcSetup(PWM_CHANNEL_3, freq, resolution); 

   // Init I2C bus
   Wire.begin();
   this->_last_state = PARKING;
   this->ToState(SLEEPING);
   this->objBoxMover.ParkArms(true);
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


void GarmentBot::onDetectedMark(uint16_t mapsite_id){
   MapSite current_mapsite;
   if (this->objMapNavigator.FetchSite(mapsite_id, &current_mapsite)){
         // the mark is being managered via map navigator.
      switch (current_mapsite.task)
      {
      case MapSite::TASK::FOLLOW_LEFT:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = false;
         this->ToState(FAST_MOVING);
         break;
      case MapSite::TASK::FOLLLOW_RIGHT:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->ToState(FAST_MOVING);
         break;
      case MapSite::TASK::LOADING:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_mapsite.task = MapSite::TASK::LOADING;
         this->ToState(PARKING);
         break;
      case MapSite::TASK::UNLOADING:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_mapsite.task = MapSite::TASK::UNLOADING;
         this->ToState(PARKING);
         break;
      case MapSite::TASK::SLEEPING:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_mapsite.task = MapSite::TASK::SLEEPING;
         this->ToState(PARKING);
         break;
      case MapSite::TASK::CHARGING:
         this->objRemoteSensor.ObjTrackSensor.FollowRightTrack = true;
         this->__current_mapsite.task = MapSite::TASK::CHARGING;
         this->ToState(PARKING);         
      default:
         break;
      }
   }
}

void GarmentBot::SpinOnce(){
   int distance_to_full_park = 100;
   bool loading_finished = true;
   bool unloading_finished = true;
   uint16_t mapsite_id = 0;
   bool found_obstacle = false;
   bool found_slowdown_mark = false;
   int track_error = 0; // this->objTrackSensor.ReadError_FromRight(&RxBuffer[0]);
   int position_error = 100;

//    this->onMqttReceived();
   this->objBoxMover.SpinOnce();

   switch (this->_State)
   {
   case FAST_MOVING:
        if (found_obstacle)
            this->ToState(FAST_MOVING_PAUSED);
        // check if see the mark of slow-down.
        else if (found_slowdown_mark)
            this->ToState(SLOW_MOVING);
        else
            this->objTwinWheel.MoveForward(track_error);
        break;
    case FAST_MOVING_PAUSED:
        if (!found_obstacle)
            this->ToState(FAST_MOVING);
        break;
    case SLOW_MOVING:
        if (found_obstacle)
            this->ToState(SLOW_MOVING_PAUSED);
        else {
            //try to read RFID
            mapsite_id = this->objRemoteSensor.ObjRfidReader.last_card_id; 
            if (mapsite_id > 0){
                // got the mark
                this->onDetectedMark(mapsite_id);
            }
        }
        break;
   case SLOW_MOVING_PAUSED:
      if (!found_obstacle)
         this->ToState(SLOW_MOVING);
      break;
   case PARKING:
        // try to finish parking
        if (distance_to_full_park == 0)
            this->ToState(PARKED);
        else{
            // position_error = this->objPositionSensor.ReadError_FromRight();
            this->objTwinWheel.SetTargetSpeed(position_error);
        }
        break;
   case PARKING_PAUSED:
        if (!found_obstacle)
            this->ToState(PARKING);
            break;
   case PARKED:
        if (this->__current_mapsite.LOADING)
            this->ToState(LOADING);
        else if (this->__current_mapsite.UNLOADING)
            this->ToState(UNLOADING);
        else if (this->__current_mapsite.SLEEPING)
            this->ToState(SLEEPING);
        else if (this->__current_mapsite.CHARGING)
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

void GarmentBot::ToState(GARMENTBOT_STATE state){
   if (state == this->_State) return;
   switch(state){
      case SLEEPING:
            this->objTwinWheel.Stop();
            break;
      case FAST_MOVING:
            this->objTwinWheel.SetTargetSpeed(220);
            break;
      case SLOW_MOVING:
            this->objTwinWheel.SetTargetSpeed(100);
            break;
      case PARKING:
            this->objTwinWheel.SetTargetSpeed(20);
            break;
      case LOADING:
            this->objBoxMover.LoadBox();
            break;
      case UNLOADING:
            this->objBoxMover.UnloadBox();
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

void GarmentBot::Test(int test_id){
   if (test_id == 1) this->objBoxMover.LoadBox();
   if (test_id == 2) this->objBoxMover.UnloadBox();
   if (test_id==10) {
        int track_error = 0;
        this->objTwinWheel.MoveForward(track_error);
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
   this->objMapNavigator.AddSite(1, MapSite::TASK::FOLLOW_LEFT);
   this->objMapNavigator.AddSite(2, MapSite::TASK::LOADING);
   this->objMapNavigator.AddSite(3, MapSite::TASK::UNLOADING);
   this->objMapNavigator.AddSite(4, MapSite::TASK::SLEEPING);

}

#endif