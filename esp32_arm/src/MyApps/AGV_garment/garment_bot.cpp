#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT
#include "garment_bot.h"




void GarmentBot::SpinOnce(){
   // Read battery


    // delay(1000);
   // Read RFID
   // Deal with MQTT
   switch  (this->_State){
      case MOVING:
         this->SpinOnce_Working();
      default:
         break;
   }
}


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
   
   this->SetMode(SLEEP);
   this->boxMover.ParkArms(true);
}


// void GarmentBot::Init_Linkage(IrEncoderHelper* sensorHelper){
   // this->agv.leftWheel->LinkDriver(&this->objLeftWheelBridge);
// }

void GarmentBot::SpinOnce_Working(){
   if (false){
		// Found Obstacle !
		this->agv.Stop();
	
	}else if (false){
		// on loading
	}else if (false){
		// on unloading
	}else{
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
			this->agv.Stop();
		}

		int track_error = this->objTrackSensor.ReadError_FromRight(&RxBuffer[0]);
		this->agv.MoveForward(track_error);
	}
}

//
//  The Command comes from MQTT, Buttons, MarkSensor, etc.
void GarmentBot::ExecuteCommand(int topic, int payload){
   
}


void GarmentBot::SetMode(GARMENTBOT_MODE mode){
   this->_State = mode;
   Serial.print("\n GarmentBot::SetMode()" );
   Serial.println(mode);
   switch( mode){
      case SLEEP:
         this->agv.Stop();
         break;
      case  MOVING:
         this->agv.SetTargetSpeed(220);
        //  this->agv.MoveForward();
         this->_State = MOVING;
         break;
      default:
         break;
   }

   
}

void GarmentBot::Test(int test_id){
    if (test_id == 1) this->boxMover.LoadBox();
    if (test_id == 2) this->boxMover.UnloadBox();
    if (test_id==10) {
        int track_error = 0;
        this->agv.MoveForward(track_error);
    }
}



#endif