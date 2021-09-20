#include "DCDriverHBridge.h"

DCDriverHBridge::DCDriverHBridge(int pinA, int pinB){
  // Pin initialization
  __pinA = pinA;
  __pinB = pinB;

  // default power-supply value
  // voltage_power_supply = DEF_POWER_SUPPLY;
  // voltage_limit = NOT_SET;

}

// // enable motor driver
// void  DCDriverHBridge::enable(){
//     // enable_pin the driver - if enable_pin pin available
//     // set zero to PWM
//     // setPwm(0,0,0);
// }

// // disable motor driver
// void DCDriverHBridge::disable()
// {
//   // set zero to PWM
//   // setPwm(0, 0, 0);
//   // // disable the driver - if enable_pin pin available
//   // if ( _isset(enableA_pin) ) digitalWrite(enableA_pin, LOW);
//   // if ( _isset(enableB_pin) ) digitalWrite(enableB_pin, LOW);
//   // if ( _isset(enableC_pin) ) digitalWrite(enableC_pin, LOW);

// }

// init hardware pins   
int DCDriverHBridge::init() {
  // PWM pins
  pinMode(__pinA, OUTPUT);
  pinMode(__pinB, OUTPUT);

  // sanity check for the voltage limit configuration
  // if(!_isset(voltage_limit) || voltage_limit > voltage_power_supply) voltage_limit =  voltage_power_supply;

  // Set the pwm frequency to the pins
  // hardware specific function - depending on driver and mcu
  // _configure3PWM(pwm_frequency, pwmA, pwmB, pwmC);
  return 0;
}



// Set voltage to the pwm pin
// void DCDriverHBridge::setPhaseState(int sa, int sb, int sc) {  
// }

// Set voltage to the pwm pin
// void DCDriverHBridge::setPwm(float U_coil) {
  
//   // limit the voltage in driver
//   U_coil= _constrain(U_coil, 0.0, voltage_limit);
//   // calculate duty cycle
//   // limited in [0,1]
//   // duty_cycle = _constrain(U_coil / voltage_power_supply, 0.0 , 1.0 );

//   // hardware specific writing
//   // hardware specific function - depending on driver and mcu
//   // _writeDutyCycle3PWM(dc_a, dc_b, dc_c, pwmA, pwmB, pwmC);
// }