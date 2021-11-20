#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT
#include "garment_bot.h"

#define PIN_HOME_SENSOR_2130 32

#define PIN_LEFT_WHEEL_DC_MOTOR_A 12
#define PIN_LEFT_WHEEL_DC_MOTOR_B 14
#define PIN_LEFT_WHEEL_DC_MOTOR_ENABLE 25
#define PIN_RIGHT_WHEEL_DC_MOTOR_A 26
#define PIN_RIGHT_WHEEL_DC_MOTOR_B 27
#define PIN_RIGHT_WHEEL_DC_MOTOR_ENABLE 33

#define PIN_VERTICAL_DC_MOTOR_A 17 //16
#define PIN_VERTICAL_DC_MOTOR_B 5  //4
#define PIN_VERTICAL_DC_MOTOR_ENABLE 15  //2
#define PIN_ANGLE_DC_MOTOR_A 16  // 17
#define PIN_ANGLE_DC_MOTORB 4    //5 
#define PIN_ANGLE_DC_MOTOR_ENABLE 2  //15

#define VERTICAL_ENDSTOP_NORTH 23
#define VERTICAL_ENDSTOP_SOUTH 13

#define ANGLE_ENDSTOP_NORTH 19
#define ANGLE_ENDSTOP_SOUTH 18

#define PWM_CHANNEL_0 0
#define PWM_CHANNEL_1 1
#define PWM_CHANNEL_2 2
#define PWM_CHANNEL_3 3


void GarmentBot::Init(){
   // Setting PWM properties
   const int freq = 30000;
   const int resolution = 8;   // so max pwm speed is 255
   ledcSetup(PWM_CHANNEL_0, freq, resolution); // configure LED PWM functionalitites
   ledcSetup(PWM_CHANNEL_1, freq, resolution); 
   ledcSetup(PWM_CHANNEL_2, freq, resolution); 
   ledcSetup(PWM_CHANNEL_3, freq, resolution); 
   // Init AGV
   objLeftWheelBridge.Init(PWM_CHANNEL_0, PIN_LEFT_WHEEL_DC_MOTOR_ENABLE, PIN_LEFT_WHEEL_DC_MOTOR_A, PIN_LEFT_WHEEL_DC_MOTOR_B);
   objRightWheelBridge.Init(PWM_CHANNEL_1, PIN_RIGHT_WHEEL_DC_MOTOR_ENABLE, PIN_RIGHT_WHEEL_DC_MOTOR_A, PIN_RIGHT_WHEEL_DC_MOTOR_B);
   PIDController* speed_pid = new PIDController(1.0f, 1.0f, 0.0f ,80.0f, 100.0f);
   this->agv_21a.leftWheel.LinkDriver(&this->objLeftWheelBridge);
   this->agv_21a.rightWheel.LinkDriver(&this->objRightWheelBridge);
   this->agv_21a.LinkTrackSensor(&this->objTrackSensor_i2c);
   this->agv_21a.LinkPid(speed_pid);

   // Init Robot
   this->objVerticalBridge.Init(PWM_CHANNEL_2, PIN_VERTICAL_DC_MOTOR_ENABLE, PIN_VERTICAL_DC_MOTOR_A, PIN_VERTICAL_DC_MOTOR_B);
   this->objAngleBridge.Init(PWM_CHANNEL_3, PIN_ANGLE_DC_MOTOR_ENABLE, PIN_ANGLE_DC_MOTOR_A, PIN_ANGLE_DC_MOTORB);
   this->boxMaster_21a.Init_VerticalMover(&this->objVerticalMover, VERTICAL_ENDSTOP_NORTH, VERTICAL_ENDSTOP_SOUTH, &this->objVerticalBridge);
   this->boxMaster_21a.Init_AngleMover(&this->objAngleMover, ANGLE_ENDSTOP_NORTH, ANGLE_ENDSTOP_SOUTH, &this->objAngleBridge);

   this->SetMode(SLEEP);
}


// void GarmentBot::Init_Linkage(IrEncoderHelper* sensorHelper){
   // this->agv_21a.leftWheel->LinkDriver(&this->objLeftWheelBridge);
// }

void GarmentBot::SpinOnce_Working(){
   if (false){
		// Found Obstacle !
		this->agv_21a.Stop();
	
	}else if (false){
		// on loading
	}else if (false){
		// on unloading
	}else{
		// Moving follow the track.
		this->agv_21a.MoveForward();
	}
}

void GarmentBot::SpinOnce(){
   switch  (this->_State){
      case MOVING:
         this->SpinOnce_Working();
      default:
         break;
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
         this->agv_21a.Stop();
         break;
      case  MOVING:
         this->agv_21a.SetTargetSpeed(220);
         this->agv_21a.MoveForward();
         this->_State = MOVING;
         break;
      default:
         break;
   }

   
}





#endif