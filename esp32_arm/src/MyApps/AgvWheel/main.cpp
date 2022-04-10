#include "all_devices.h"
#ifdef I_AM_AGV_WHEEL
#include <SimpleFOC.h>


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

// Hall sensor instance
// HallSensor(int hallA, int hallB , int hallC , int pp)
//  - hallA, hallB, hallC    - HallSensor A, B and C pins
//  - pp                     - pole pairs
HallSensor sensor = HallSensor(19, 18, 5, 15);

// Interrupt routine initialization
// channel A and B callbacks
void doA(){sensor.handleA();}
void doB(){sensor.handleB();}
void doC(){sensor.handleC();}


// current sensor
// shunt resistor value
// gain value
// pins phase A,B
// LowsideCurrentSense current_sense = LowsideCurrentSense(0.01, 50.0, 1, 2);

BLDCDriver6PWM driver = BLDCDriver6PWM(12,14, 27,26, 25,33, 13);

//  BLDCMotor(int pp, (optional R))
//  - pp  - pole pair number
//  - R   - phase resistance value - optional
BLDCMotor motor = BLDCMotor(15 , 2.8);

void setup() {
    // monitoring port
    Serial.begin(115200);

    pinMode(PIN_DC_CAL, OUTPUT);
    digitalWrite(PIN_DC_CAL, LOW);
    pinMode(PIN_GAIN, OUTPUT);
    digitalWrite(PIN_GAIN, LOW);

    pinMode(PIN_POWER_IS_GOOD, INPUT_PULLUP); 
    pinMode(PIN_FAULT, INPUT_PULLUP);
    pinMode(PIN_OverCurrentTermperatureWarning, INPUT_PULLUP);

    sensor.pullup = Pullup::USE_INTERN;
    sensor.init();
    sensor.enableInterrupts(doA, doB, doC);
    Serial.println("Sensor ready");

    // initialise the current sensing
    // current_sense.init();
    // motor.linkCurrentSense(&current_sense);


    // pwm frequency to be used [Hz]
    driver.pwm_frequency = 40000;
    // power supply voltage [V]
    driver.voltage_power_supply = 12;
    // Max DC voltage allowed - default voltage_power_supply
    driver.voltage_limit = 12;
    // daad_zone [0,1] - default 0.02 - 2%
    driver.dead_zone = 0.05;

    // driver init
    driver.init();

    // enable driver
    driver.enable();

    // linking the driver to the motor
    motor.linkDriver(&driver);

    // controller configuration based on the control type 
    // velocity PID controller parameters
    // default P=0.5 I = 10 D = 0
    motor.PID_velocity.P = 0.2;
    motor.PID_velocity.I = 20;
    motor.PID_velocity.D = 0.001;
    // jerk control using voltage voltage ramp
    // default value is 300 volts per sec  ~ 0.3V per millisecond
    motor.PID_velocity.output_ramp = 1000;

    // velocity low pass filtering
    // default 5ms - try different values to see what is the best. 
    // the lower the less filtered
    motor.LPF_velocity.Tf = 0.01;

    // setting the limits
    // either voltage
    motor.voltage_limit = 10; // Volts - default driver.voltage_limit
    // of current 
    motor.current_limit = 2; // Amps - default 0.2Amps

	motor.initFOC();


    _delay(1000);
}

float target_velocity = 2; // 2Rad/s ~ 20rpm

void loop() {
    // IMPORTANT - call as frequently as possible
    // update the sensor values 
    sensor.update();
    // main FOC algorithm function
    
    motor.loopFOC();
    // display the angle and the angular velocity to the terminal
    // Motion control function
    motor.move(target_velocity);
    Serial.print(sensor.getAngle());
    Serial.print("\t");
    Serial.print(sensor.getVelocity());
    Serial.print("\t");
    Serial.print(digitalRead(PIN_POWER_IS_GOOD));
    Serial.print("\t");
    Serial.print(digitalRead(PIN_FAULT));
    Serial.print("\t");
    Serial.print(digitalRead(PIN_OverCurrentTermperatureWarning));
    Serial.print("\t");
    // Serial.print(digitalRead(PIN_GAIN));
    // Serial.print("\t");
    Serial.println();
}

#endif