#ifndef __ACTUATOR_BASE_H_
#define __ACTUATOR_BASE_H_

#include "Arduino.h"


#include <SimpleFOC.h>
// #include "SimpleFOC/common/pid.h"
// #include "SimpleFOC/common/base_classes/Sensor.h"
// #include "CNC/Sensor/IREncoderHelper.h"
#include "CNC/Sensor/SensorHelperBase.h"
// #include "SimpleFOC/common/defaults.h"
#include "CNC/Actuator/DriverBase.h"

/**
 *  Motiron control type
 */
// enum MotionControlType{
//   torque,//!< Torque control
//   velocity,//!< Velocity motion control
//   angle,//!< Position/angle motion control
//   velocity_openloop,
//   angle_openloop
// };


/**
  Senario HasDriver HasSensor       Explain
   A        YES       YES         This simplest machine, with normal actuator
   B        YES       NO          Like camera sensor. measure many dimensions.
   C        HALF      YES         Like TeensyStep, The movement is started from controller, not the driver.
   F        NO        NO          Very wired, like ?
   
   **B  There are a group sensors,  reflect a group of actuators. Must has a function like
        All_ActuatorPos ap = ConvertFrom (SensorsPos* sensorPos){};
        All_ActuatorPos ap = ConvertFrom (CameraImage* image){};
*/
class ActuatorBase
{
    public:
        ActuatorBase(){};
        void SetTargetAbs(float pos);
        void SetTargetSpeed(float speed);
        float GetCurrentPos();
        void SetCurrentPos(float position);

        void LinkSensorHelper(SensorHelperBase* sensorHelper){this->sensorHelper=sensorHelper;};
        void LinkDriver(DriverBase* driver){
            this->driver = driver;
        };
        void SayHello(){
            Serial.println(" Hello world, I am an instance of  Class ActuatorBase ");
        };

        char AxisName;
        // Sensor* sensor; 
        SensorHelperBase* sensorHelper;
        DriverBase* driver;
        bool IsRunning=false;
        float positionTolerance = 1.0f;
        //Some properties like below: Might be useful in feature.
        float MaxSpeed = 1;
        // float Weight = 0;
        // float MaxAcceleration = 2;
        // Mass gravity = (1,2,3);
        float TargetSpeed;

    protected:

    private:

};


#endif
