#include "../select_app.h"
// #include "all_devices.h"
#ifdef I_AM_DRV8302

#include <SimpleFOC.h>
#include <SoftwareSerial.h>
#include "main_hw.h"
#include "Arduino.h"



// BLDC driver instance
BLDCDriver3PWM driver = BLDCDriver3PWM(PIN_PHASE_A_HIGH, PIN_PHASE_B_HIGH, PIN_PHASE_C_HIGH, PIN_DRV8302_ENABLE);

void setup() {
  Serial.begin(115200);
  Serial.println("Hi world");
    // For output
    pinMode(PIN_DC_CAL, OUTPUT);
    digitalWrite(PIN_DC_CAL, LOW);
    pinMode(PIN_GAIN, OUTPUT);
    digitalWrite(PIN_GAIN, LOW);

    pinMode(PIN_FAULT,INPUT_PULLUP);
    pinMode(PIN_OverCurrentTermperatureWarning, INPUT_PULLUP);

    
  // pwm frequency to be used [Hz]
  driver.pwm_frequency = 40000;
  // power supply voltage [V]
  driver.voltage_power_supply = 12;
  // Max DC voltage allowed - default voltage_power_supply
  driver.voltage_limit = 12;

  // driver init
  driver.init();

  // enable driver
  driver.enable();

  _delay(1000);
}

float a = 3;
float b= 6;
float c = 5;

void loop_a() {
    // setting pwm
    // phase A: 3V, phase B: 6V, phase C: 5V
    driver.setPwm(a,a,a);
    Serial.print(a);
    Serial.print("  ");
    _delay(1);
    a += 0.01;
    if (a>=6){
        a=0;
    }
}
void loop_b(){
    int ddd= 10000;
        // phase (A: 3V, B: 6V, C: high impedance )  
    // set the phase C in high impedance mode - disabled or open
    driver.setPhaseState(_ACTIVE , _ACTIVE , _HIGH_Z); // _HIGH_Z or _HIGH_IMPEDANCE
    driver.setPwm(6, 3, 0); 
    _delay(ddd);

    // phase (A: high impedance, B: 3V, C: 6V )  
    // set the phase A in high impedance mode - disabled or open
    driver.setPhaseState(_HIGH_IMPEDANCE, _ACTIVE, _ACTIVE);
    driver.setPwm(0, 6, 3);
    _delay(ddd);

    // phase (A: 3V, B: high impedance, C: 6V )  
    // set the phase B in high impedance mode - disabled or open
    driver.setPhaseState(_ACTIVE , _HIGH_IMPEDANCE, _ACTIVE);
    driver.setPwm(3, 0, 6);
    _delay(ddd);


}

void loop(){
  loop_b();
  if(digitalRead(PIN_FAULT) == LOW)
    Serial.println("PIN_FAULT");
  if(digitalRead(PIN_OverCurrentTermperatureWarning)==LOW)
    Serial.println("OverCurrent");
}
#endif