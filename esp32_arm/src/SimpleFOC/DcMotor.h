#ifndef DCMotor_h
#define DCMotor_h

#include "Arduino.h"
#include "common/base_classes/FOCMotor.h"
#include "common/base_classes/Sensor.h"
#include "common/base_classes/DCDriver.h"
#include "common/foc_utils.h"
#include "common/time_utils.h"
#include "common/defaults.h"

/**
 DC motor class
*/
class DCMotor: public FOCMotor
{
  public:
    /**
     DCMotor class constructor
     */ 
    DCMotor();
    
    /**
     * Function linking a motor and a foc driver 
     * 
     * @param driver DCDriver class implementing all the hardware specific functions necessary PWM setting
     */
    void linkDriver(DCDriver* driver);

    DCDriver* driver; 
    
    /**  Motor hardware init function */
  	void init() override;
  	void disable() override;
    void enable() override;

    /**
     * Function initializing FOC algorithm
     * and aligning sensor's and motors' zero position 
     */  
    int initFOC( float zero_electric_offset = NOT_SET , Direction sensor_direction = Direction::CW) override;
    /**
     * Function running FOC algorithm in real-time
     * it calculates the gets motor angle and sets the appropriate voltages 
     * to the phase pwm signals
     * - the faster you can run it the better Arduino UNO ~1ms, Bluepill ~ 100us
     */ 
    void loopFOC() override;

    /**
     * Function executing the control loops set by the controller parameter of the DCMotor.
     * 
     * @param target  Either voltage, angle or velocity based on the motor.controller
     *                If it is not set the motor will use the target set in its variable motor.target
     * 
     * This function doesn't need to be run upon each loop execution - depends of the use case
     */
    void move(float target = NOT_SET) override;
    

  private:

        
    // Open loop motion control    
    /**
     * Function (iterative) generating open loop movement for target velocity
     * it uses voltage_limit variable
     * 
     * @param target_velocity - rad/s
     */
    float velocityOpenloop(float target_velocity);
    /**
     * Function (iterative) generating open loop movement towards the target angle
     * it uses voltage_limit and velocity_limit variables
     * 
     * @param target_angle - rad
     */
    float angleOpenloop(float target_angle);
    // open loop variables
    long open_loop_timestamp;
};


#endif
