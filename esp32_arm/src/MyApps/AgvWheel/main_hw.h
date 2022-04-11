#pragma once




#define PIN_SERIAL_TX 22
#define PIN_SERIAL_RX 22

// When DC_CAL is high, device shorts inputs of shunt amplifiers and disconnects loads. 
// DC offset calibration can be done through external microcontroller.
#define PIN_DC_CAL 17

// An open drain output with external pullup resistor required. 
//Asserts low if buck output voltage is low due to thermal shutdown, dropout, overvoltage, 
// or EN_BUCK shut down
// The BUCK has a power good comparator (PWRGD) which asserts 
//     when the regulated output voltage is less than 92% or greater than 109% of the nominal output voltage. 

// !!!  The PWRGD pin is an open-drain output that deasserts 
//     when the VSENSE pin voltage is between 94% and 107% of the nominal output voltage, 
//     allowing the pin to transition high when a pullup resistor is used

#define PIN_POWER_IS_GOOD 15

// Gain selection for integrated current shunt amplifiers. 
// If GAIN = LOW, the internal current shunt amplifiers have a gain of 10V/V. 
// If GAIN = HIGH, the current shunt amplifiers have a gain of 40V/V.
#define PIN_GAIN 16
// Note that nFAULT is an open-drain signal. 
//  nFAULT goes high when gate driver is ready for PWM signal 
//  (internal EN_GATE goes high) during start-up.
#define PIN_FAULT 4
#define PIN_OverCurrentTermperatureWarning 2


#define PIN_HALL_SENSOR_U 19
#define PIN_HALL_SENSOR_V 18
#define PIN_HALL_SENSOR_W 5

#define PIN_PHASE_A_HIGH 12
#define PIN_PHASE_A_LOW 14

#define PIN_PHASE_B_HIGH 27
#define PIN_PHASE_B_LOW 26

#define PIN_PHASE_C_HIGH 25
#define PIN_PHASE_C_LOW 33

#define PIN_DRV8302_ENABLE 13

#define MOTOR_POLE_PAIRS 15

#define PIN_CURRENT_SENSOR_A 39
#define PIN_CURRENT_SENSOR_B 36
