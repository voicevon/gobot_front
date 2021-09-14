#include "DCMotor.h"

// DCMotor( int pp , float R)
DCMotor::DCMotor():FOCMotor(){
}


/**
	Link the driver which controls the motor
*/
void DCMotor::linkDriver(DCDriver* _driver) {
  driver = _driver;
}

// init hardware pins
void DCMotor::init() {
  if(monitor_port) monitor_port->println(F("MOT: Init"));

  // if no current sensing and the user has set the phase resistance of the motor use current limit to calculate the voltage limit
  if( !current_sense && _isset(phase_resistance)) {
    float new_voltage_limit = current_limit * (phase_resistance); // v_lim = current_lim / (3/2 phase resistance) - worst case
    // use it if it is less then voltage_limit set by the user
    voltage_limit = new_voltage_limit < voltage_limit ? new_voltage_limit : voltage_limit;
  }
  // sanity check for the voltage limit configuration
  // if(voltage_limit > driver->voltage_limit) voltage_limit =  driver->voltage_limit;
  // constrain voltage for sensor alignment
  if(voltage_sensor_align > voltage_limit) voltage_sensor_align = voltage_limit;

  // update the controller limits
  if(current_sense){
    // current control loop controls voltage
    PID_current_q.limit = voltage_limit;
    PID_current_d.limit = voltage_limit;
    // velocity control loop controls current
    PID_velocity.limit = current_limit;
  }else if(!current_sense && _isset(phase_resistance)){
    PID_velocity.limit = current_limit;
  }else{
    PID_velocity.limit = voltage_limit;
  }
  P_angle.limit = velocity_limit;

  _delay(500);
  // enable motor
  if(monitor_port) monitor_port->println(F("MOT: Enable driver."));
  enable();
  _delay(500);
}


// Must have overide function here, Keep them blank.
void DCMotor::disable() {}
void DCMotor::enable() {}
// 
/**
  FOC functions
*/
// FOC initialization function
int  DCMotor::initFOC( float zero_electric_offset, Direction _sensor_direction) {
  int exit_flag = 1;
  // align motor if necessary
  // alignment necessary for encoders!
  if(_isset(zero_electric_offset)){
    // abosolute zero offset provided - no need to align
    zero_electric_angle = zero_electric_offset;
    // set the sensor direction - default CW
    sensor_direction = _sensor_direction;
  }

  // sensor and motor alignment - can be skipped
  // by setting motor.sensor_direction and motor.zero_electric_angle
  _delay(500);
  if(sensor){
    // exit_flag *= alignSensor();
    // added the shaft_angle update
    shaft_angle = sensor->getAngle();
  }else if(monitor_port) monitor_port->println(F("MOT: No sensor."));

  // aligning the current sensor - can be skipped
  // checks if driver phases are the same as current sense phases
  // and checks the direction of measuremnt.
  _delay(500);
  if(exit_flag){
    // if(current_sense) exit_flag *= alignCurrentSense();
    // else if(monitor_port) monitor_port->println(F("MOT: No current sense."));
  }

  if(exit_flag){
    if(monitor_port) monitor_port->println(F("MOT: Ready."));
  }else{
    if(monitor_port) monitor_port->println(F("MOT: Init FOC failed."));
    disable();
  }

  return exit_flag;
}



// Iterative function looping FOC algorithm, setting Uq on the Motor
// The faster it can be run the better
void DCMotor::loopFOC() {
  // if open-loop do nothing
  if( controller==MotionControlType::angle_openloop || controller==MotionControlType::velocity_openloop ) return;
  // shaft angle
  shaft_angle = shaftAngle(); // read value even if motor is disabled to keep the monitoring updated

  // if disabled do nothing
  if(!enabled) return;

  // electrical angle - need shaftAngle to be called first
  electrical_angle = electricalAngle();

  switch (torque_controller) {
    case TorqueControlType::voltage:
      // no need to do anything really
      break;
    case TorqueControlType::dc_current:
      if(!current_sense) return;
      // read overall current magnitude
      current.q = current_sense->getDCCurrent(electrical_angle);
      // filter the value values
      current.q = LPF_current_q(current.q);
      // calculate the phase voltage
      voltage.q = PID_current_q(current_sp - current.q);
      voltage.d = 0;
      break;
    case TorqueControlType::foc_current:
      if(!current_sense) return;
      // read dq currents
      current = current_sense->getFOCCurrents(electrical_angle);
      // filter values
      current.q = LPF_current_q(current.q);
      current.d = LPF_current_d(current.d);
      // calculate the phase voltages
      voltage.q = PID_current_q(current_sp - current.q);
      voltage.d = PID_current_d(-current.d);
      break;
    default:
      // no torque control selected
      if(monitor_port) monitor_port->println(F("MOT: no torque control selected!"));
      break;
  }

  // set the phase voltage - FOC heart function :)
  // setPhaseVoltage(voltage.q, voltage.d, electrical_angle);
}

// Iterative function running outer loop of the FOC algorithm
// Behavior of this function is determined by the motor.controller variable
// It runs either angle, velocity or torque loop
// - needs to be called iteratively it is asynchronous function
// - if target is not set it uses motor.target value
void DCMotor::move(float new_target) {
  // get angular velocity
  shaft_velocity = shaftVelocity(); // read value even if motor is disabled to keep the monitoring updated

  // if disabled do nothing
  if(!enabled) return;
  // downsampling (optional)
  if(motion_cnt++ < motion_downsample) return;
  motion_cnt = 0;
  // set internal target variable
  if(_isset(new_target)) target = new_target;

  switch (controller) {
    case MotionControlType::torque:
      if(torque_controller == TorqueControlType::voltage) // if voltage torque control
        if(!_isset(phase_resistance))  voltage.q = target;
        else voltage.q =  target*phase_resistance;
      else
        current_sp = target; // if current/foc_current torque control
      break;
    case MotionControlType::angle:
      // angle set point
      shaft_angle_sp = target;
      // calculate velocity set point
      shaft_velocity_sp = P_angle( shaft_angle_sp - shaft_angle );
      // calculate the torque command
      current_sp = PID_velocity(shaft_velocity_sp - shaft_velocity); // if voltage torque control
      // if torque controlled through voltage
      if(torque_controller == TorqueControlType::voltage){
        // use voltage if phase-resistance not provided
        if(!_isset(phase_resistance))  voltage.q = current_sp;
        else  voltage.q = current_sp*phase_resistance;
        voltage.d = 0;
      }
      break;
    case MotionControlType::velocity:
      // velocity set point
      shaft_velocity_sp = target;
      // calculate the torque command
      current_sp = PID_velocity(shaft_velocity_sp - shaft_velocity); // if current/foc_current torque control
      // if torque controlled through voltage control
      if(torque_controller == TorqueControlType::voltage){
        // use voltage if phase-resistance not provided
        if(!_isset(phase_resistance))  voltage.q = current_sp;
        else  voltage.q = current_sp*phase_resistance;
        voltage.d = 0;
      }
      break;
    case MotionControlType::velocity_openloop:
      // velocity control in open loop
      shaft_velocity_sp = target;
      voltage.q = velocityOpenloop(shaft_velocity_sp); // returns the voltage that is set to the motor
      voltage.d = 0;
      break;
    case MotionControlType::angle_openloop:
      // angle control in open loop
      shaft_angle_sp = target;
      voltage.q = angleOpenloop(shaft_angle_sp); // returns the voltage that is set to the motor
      voltage.d = 0;
      break;
  }
}



// Function (iterative) generating open loop movement for target velocity
// - target_velocity - rad/s
// it uses voltage_limit variable
float DCMotor::velocityOpenloop(float target_velocity){
  // get current timestamp
  unsigned long now_us = _micros();
  // calculate the sample time from last call
  float Ts = (now_us - open_loop_timestamp) * 1e-6;
  // quick fix for strange cases (micros overflow + timestamp not defined)
  if(Ts <= 0 || Ts > 0.5) Ts = 1e-3;

  // calculate the necessary angle to achieve target velocity
  shaft_angle = _normalizeAngle(shaft_angle + target_velocity*Ts);
  // for display purposes
  shaft_velocity = target_velocity;

  // use voltage limit or current limit
  float Uq = voltage_limit;
  if(_isset(phase_resistance)) Uq =  current_limit*phase_resistance;

  // set the maximal allowed voltage (voltage_limit) with the necessary angle
  // setPhaseVoltage(Uq,  0, _electricalAngle(shaft_angle, pole_pairs));

  // save timestamp for next call
  open_loop_timestamp = now_us;

  return Uq;
}

// Function (iterative) generating open loop movement towards the target angle
// - target_angle - rad
// it uses voltage_limit and velocity_limit variables
float DCMotor::angleOpenloop(float target_angle){
  // get current timestamp
  unsigned long now_us = _micros();
  // calculate the sample time from last call
  float Ts = (now_us - open_loop_timestamp) * 1e-6;
  // quick fix for strange cases (micros overflow + timestamp not defined)
  if(Ts <= 0 || Ts > 0.5) Ts = 1e-3;

  // calculate the necessary angle to move from current position towards target angle
  // with maximal velocity (velocity_limit)
  if(abs( target_angle - shaft_angle ) > abs(velocity_limit*Ts)){
    shaft_angle += _sign(target_angle - shaft_angle) * abs( velocity_limit )*Ts;
    shaft_velocity = velocity_limit;
  }else{
    shaft_angle = target_angle;
    shaft_velocity = 0;
  }


  // use voltage limit or current limit
  float Uq = voltage_limit;
  if(_isset(phase_resistance)) Uq =  current_limit*phase_resistance;
  // set the maximal allowed voltage (voltage_limit) with the necessary angle
  // setPhaseVoltage(Uq,  0, _electricalAngle(shaft_angle, pole_pairs));

  // save timestamp for next call
  open_loop_timestamp = now_us;

  return Uq;
}
