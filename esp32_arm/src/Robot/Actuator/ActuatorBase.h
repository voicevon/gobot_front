#ifndef __ACTUATOR_BASE_H_
#define __ACTUATOR_BASE_H_

#include "Arduino.h"
// #include "Sensor.h"
// #include "CurrentSense.h"

// #include "../time_utils.h"
// #include "../foc_utils.h"
// #include "../defaults.h"
// #include "../pid.h"


#include "SimpleFOC/common/pid.h"
#include "SimpleFOC/common/base_classes/Sensor.h"
#include "SimpleFOC/common/defaults.h"

// #include "../lowpass_filter.h"


// monitoring bitmap
#define _MON_TARGET 0b1000000 // monitor target value
#define _MON_VOLT_Q 0b0100000 // monitor voltage q value
#define _MON_VOLT_D 0b0010000 // monitor voltage d value
#define _MON_CURR_Q 0b0001000 // monitor current q value - if measured
#define _MON_CURR_D 0b0000100 // monitor current d value - if measured
#define _MON_VEL    0b0000010 // monitor velocity value
#define _MON_ANGLE  0b0000001 // monitor angle value

/**
 *  Motiron control type
 */
enum MotionControlType{
  torque,//!< Torque control
  velocity,//!< Velocity motion control
  angle,//!< Position/angle motion control
  velocity_openloop,
  angle_openloop
};

// /**
//  *  Motiron control type
//  */
// enum TorqueControlType{
//   voltage, //!< Torque control using voltage
//   dc_current, //!< Torque control using DC current (one current magnitude)
//   foc_current //!< torque control using dq currents
// };

// /**
//  *  FOC modulation type
//  */
// enum FOCModulationType{
//   SinePWM, //!< Sinusoidal PWM modulation
//   SpaceVectorPWM, //!< Space vector modulation method
//   Trapezoid_120,
//   Trapezoid_150
// };

/**
  1.  An actuator DOES NOT know how to move, it's driven by a driver,
  2.  An actuator DOES KNOW the currently position it is at.
        The position might comes from encoder,   or stepper-driver ? servo-driver?
  3.  Know the target position.  
*/
class ActuatorBase
{
  public:
    ActuatorBase();
    void SetTargetAbs(float pos);
    float GetCurrentPos(){return 0.0;};
    void linkSensor(Sensor* sensor);

    // float shaftVelocity();
    Sensor* sensor; 
    bool IsRunning=false;

    //Some properties like below: Might be useful in feature.
    float MaxSpeed = 1;
    // float Weight = 0;
    // float MaxAcceleration = 2;
    // Mass gravity = (1,2,3);
  private:

};


#endif
