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

#define PIN_Z_DC_MOTOR_A 4
#define PIN_Z_DC_MOTOR_B 16
#define PIN_Z_DC_MOTOR_ENABLE 2
#define PIN_ANGLE_DC_MOTOR_A 17
#define PIN_ANGLE_DC_MOTORB 5
#define PIN_ANGLE_DC_MOTOR_ENABLE 15

#define PWM_CHANNEL_0 0
#define PWM_CHANNEL_1 1
#define PWM_CHANNEL_2 2
#define PWM_CHANNEL_3 3


GarmentBot::GarmentBot(){
}

void GarmentBot::Init(){
   // Setting PWM properties
   const int freq = 30000;

   const int resolution = 8;   // so max pwm speed is 255
   
   ledcSetup(PWM_CHANNEL_0, freq, resolution); // configure LED PWM functionalitites ,  should be outside?
   // Init AGV
   objLeftWheelBridge.Init(PWM_CHANNEL_1, PIN_LEFT_WHEEL_DC_MOTOR_ENABLE, PIN_LEFT_WHEEL_DC_MOTOR_A, PIN_LEFT_WHEEL_DC_MOTOR_B);
   // this->agv_21a.leftWheel.SayHello();
   this->agv_21a.leftWheel.LinkDriver(&this->objLeftWheelBridge);

   objRightWheelBridge.Init(PWM_CHANNEL_2, PIN_RIGHT_WHEEL_DC_MOTOR_ENABLE, PIN_RIGHT_WHEEL_DC_MOTOR_A, PIN_RIGHT_WHEEL_DC_MOTOR_B);
   // this->agv.leftWheel->LinkSensorHelper();
   this->agv_21a.rightWheel.LinkDriver(&this->objRightWheelBridge);
   PIDController* speed_pid = new PIDController(1.0f, 1.0f, 0.0f ,80.0f, 100.0f);
   this->agv_21a.LinkTrackSensor(&this->objTrackSensor_i2c);
   this->agv_21a.LinkPid(speed_pid);

   // Init Robot
   objZAxisBridge.Init(PWM_CHANNEL_2, PIN_ANGLE_DC_MOTOR_ENABLE, PIN_Z_DC_MOTOR_A, PIN_Z_DC_MOTOR_B);
   objAngleBridge.Init(PWM_CHANNEL_3, PIN_ANGLE_DC_MOTOR_ENABLE, PIN_ANGLE_DC_MOTOR_A, PIN_ANGLE_DC_MOTORB);

   // this->robot_21a.Init_Linkage();
   this->SetMode(SLEEP);
   Serial.print(" zzzzzzzzzzzzzzzzzz  ");
}


void GarmentBot::Init_Linkage(IrEncoderHelper* sensorHelper){
   // this->agv_21a.leftWheel->LinkDriver(&this->objLeftWheelBridge);
}

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
   switch  (this->_mode){
      case SLEEP:
         this->agv_21a.Stop();
         this->robot_21a.Stop();
         break;
      case WORKING:
         this->SpinOnce_Working();
      default:
         break;
   }
}

void GarmentBot::SetMode(GARMENTBOT_MODE mode){
   this->_mode = mode;
   Serial.print("\n GarmentBot::SetMode()" );
   Serial.println(mode);
   switch( mode){
      case SLEEP:
         this->agv_21a.Stop();
         break;
      case  WORKING:
         this->agv_21a.SetTargetSpeed(220);
         this->agv_21a.MoveForward();
         break;
      default:
         break;
   }

   
}





#endif