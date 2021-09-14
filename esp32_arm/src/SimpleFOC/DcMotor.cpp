#include "DCMotor.h"

DCMotor::DCMotor():FOCMotor(){}

// void DCMotor::linkDriver(DCDriver* _driver) {
//   driver = _driver;
// }

void DCMotor::init() {
  if(monitor_port) monitor_port->println(F("MOT: Init"));

  // if no current sensing and the user has set the phase resistance of the motor use current limit to calculate the voltage limit
  if( !current_sense && _isset(phase_resistance)) {
    float new_voltage_limit = current_limit * (phase_resistance); // v_lim = current_lim / (3/2 phase resistance) - worst case
    // use it if it is less then voltage_limit set by the user
    voltage_limit = new_voltage_limit < voltage_limit ? new_voltage_limit : voltage_limit;
  }
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
  }
}


// Must have overide function here, Keep them blank.
void DCMotor::disable() {}
void DCMotor::enable() {}
int  DCMotor::initFOC(float zero_electric_offset, Direction _sensor_direction) {}
void DCMotor::loopFOC() {}
