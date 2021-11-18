#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT
#include "garment_bot.h"

#define PIN_HOME_SENSOR_2130 32

#define PIN_LEFT_WHEEL_DC_MOTOR_A 12
#define PIN_LEFT_WHEEL_DC_MOTOR_B 14
#define PIN_LEFT_WHEEL_DC_MOTOR_ENABLE 25
#define PIN_RIGHT_WHEEL_DC_MOTOR_A 27
#define PIN_RIGHT_WHEEL_DC_MOTOR_B 26
#define PIN_RIGHT_WHEEL_DC_MOTOR_ENABLE 33

#define PIN_Z_DC_MOTOR_A 4
#define PIN_Z_DC_MOTOR_B 16
#define PIN_Z_DC_MOTOR_ENABLE 2
#define PIN_ANGLE_DC_MOTOR_A 17
#define PIN_ANGLE_DC_MOTORB 5
#define PIN_ANGLE_DC_MOTOR_ENABLE 15



GarmentBot::GarmentBot(){
   // Init AGV
   objLeftWheelBridge.Init(PIN_LEFT_WHEEL_DC_MOTOR_ENABLE, PIN_LEFT_WHEEL_DC_MOTOR_A, PIN_LEFT_WHEEL_DC_MOTOR_B);
   objRightWheelBridge.Init(PIN_RIGHT_WHEEL_DC_MOTOR_ENABLE, PIN_RIGHT_WHEEL_DC_MOTOR_A, PIN_RIGHT_WHEEL_DC_MOTOR_B);
   this->agv_2110.leftWheel->LinkDriver(&objLeftWheelBridge);
   // this->agv.leftWheel->LinkSensorHelper();
   this->agv_2110.rightWheel->LinkDriver(&objRightWheelBridge);
   PIDController* speed_pid = new PIDController(1.0f, 1.0f, 0.0f ,80.0f, 100.0f);
   this->agv_2110.LinkTrackSensor(&objTrackSensor_i2c);
   this->agv_2110.LinkWheels(&this->objLeftWheel, &this->objRightWheel);
   this->agv_2110.LinkPid(speed_pid);
   this->SetMode(SLEEP);

   // Init Robot
   objZAxisBridge.Init(PIN_ANGLE_DC_MOTOR_ENABLE, PIN_Z_DC_MOTOR_A, PIN_Z_DC_MOTOR_B);
   objAngleBridge.Init(PIN_ANGLE_DC_MOTOR_ENABLE, PIN_ANGLE_DC_MOTOR_A, PIN_ANGLE_DC_MOTORB);

   // this->robot.Init_Linkage();
}


void GarmentBot::Init_Linkage(IrEncoderHelper* sensorHelper){
   this->agv_2110.leftWheel->LinkDriver(&this->objLeftWheelBridge);
}

void GarmentBot::SpinOnce_Working(){
   if (false){
		// Found Obstacle !
		this->agv_2110.Stop();
	
	}else if (false){
		// on loading
	}else if (false){
		// on unloading
	}else{
		// Moving follow the track.
		// this->agv_2110.Move(FORWARD,50);
	}
}

void GarmentBot::SpinOnce(){
   this->agv_2110.SpinOnce();
   this->robot.SpinOnce();

   switch  (this->_mode){
      case SLEEP:
         this->agv_2110.Stop();
         this->robot.Stop();
         break;
      case WORKING:
         this->SpinOnce_Working();
      default:
         break;
   }
}

void GarmentBot::SetMode(GARMENTBOT_MODE mode){
   this->_mode = mode;
   if (mode == WORKING){
      this->agv_2110.SetTargetSpeed(200);
   }
}





#endif