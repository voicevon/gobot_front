#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT
#include "garment_bot.h"


void GarmentBot::Init(){
   // Setting PWM channels properties, Totally Esp32 has 16 channels
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
   this->boxMover.ParkArms(true);
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
// void GarmentBot::Init_Linkage(IrEncoderHelper* sensorHelper){
   // this->objTwinWheel.leftWheel->LinkDriver(&this->objLeftWheelBridge);
// }
void GarmentBot::CommuWithUppper(){
   this->objMapNavigator.AddSite(1,true,false);
   this->objMapNavigator.AddSite(2,true,true);
}

void GarmentBot::SpinOnce(){
   int distance_to_full_park = 100;
   bool loading_finished = true;
   bool unloading_finished = true;
   uint16_t mapsite_id = 0;
   MapSite current_mapsite;
   bool found_obstacle = false;
   bool found_slowdown_mark = false;
   int track_error = 0; // this->objTrackSensor.ReadError_FromRight(&RxBuffer[0]);
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
      if (true)
         this->ToState(FAST_MOVING);
      break;
   case SLOW_MOVING:
      if (true)
         this->ToState(SLOW_MOVING_PAUSED);
      else {
        //try to read RFID
        mapsite_id = this->objRemoteRfidReader; 
        if (mapsite_id > 0){
            // got the mark
            if (this->objMapNavigator.FetchSite(mapsite_id, &current_mapsite)){
                // the mark is being managered via map navigator.
                this->objTrackSensor.FollowRightTrack = current_mapsite.FollowLeft;   //TODO: unify direction.
                if (current_mapsite.ShouldPark)
                    // should  park to this station (working station or battery chariging station)
                    this->ToState(PARKING);
                else
                    // should not park here, track following direction has been set. so start fast moving.
                    this->ToState(FAST_MOVING);
            }
            else
                // the mark is NOT being managered via map navigator. 
                // should move to next site.
                this->ToState(FAST_MOVING);
        }
      }
      break;
   case SLOW_MOVING_PAUSED:
      if (true)
         this->ToState(SLOW_MOVING);
      break;
   case PARKING:
      // try to finish parking
      if (distance_to_full_park == 0)
         this->ToState(PARKED);
      break;
   case PARKING_PAUSED:
      if (true)
         this->ToState(PARKING);
         break;
   case PARKED:
      if (true)
         this->ToState(LOADING);
      else if (true)
         this->ToState(UNLOADING);
      else if (true)
         this->ToState(SLEEPING);
      else if (true)
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

//
//  The Command comes from MQTT, Buttons, MarkSensor, etc.
void GarmentBot::ExecuteCommand(int topic, int payload){
   
}


void GarmentBot::ToState(GARMENTBOT_STATE state){
   if (state == this->_State) return;
   switch(state){
      case SLEEPING:
         this->objTwinWheel.Stop();
         break;
      case  FAST_MOVING:
         this->objTwinWheel.SetTargetSpeed(220);
         break;
      case SLOW_MOVING:
         this->objTwinWheel.SetTargetSpeed(100);
         break;
      case PARKING:
         this->objTwinWheel.SetTargetSpeed(20);
         break;
      case LOADING:
         this->boxMover.LoadBox();
         break;
      case UNLOADING:
         this->boxMover.UnloadBox();
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
    if (test_id == 1) this->boxMover.LoadBox();
    if (test_id == 2) this->boxMover.UnloadBox();
    if (test_id==10) {
        int track_error = 0;
        this->objTwinWheel.MoveForward(track_error);
    }
}



#endif