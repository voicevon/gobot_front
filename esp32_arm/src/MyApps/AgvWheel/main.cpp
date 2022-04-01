#include "all_devices.h"
#ifdef I_AM_AGV_WHEEL
#include <SimpleFOC.h>

// Hall sensor instance
// HallSensor(int hallA, int hallB , int hallC , int pp)
//  - hallA, hallB, hallC    - HallSensor A, B and C pins
//  - pp                     - pole pairs
HallSensor sensor = HallSensor(2, 3, 4, 11);

// Interrupt routine initialization
// channel A and B callbacks
void doA(){sensor.handleA();}
void doB(){sensor.handleB();}
void doC(){sensor.handleC();}


// current sensor
// shunt resistor value
// gain value
// pins phase A,B
LowsideCurrentSense current_sense = LowsideCurrentSense(0.01, 50.0, 1, 2);

BLDCDriver6PWM driver = BLDCDriver6PWM(7, 2, 6, 3, 5, 4);

//  BLDCMotor(int pp, (optional R))
//  - pp  - pole pair number
//  - R   - phase resistance value - optional
BLDCMotor motor = BLDCMotor(11 , 10.5);

void setup() {
    // monitoring port
    Serial.begin(115200);
    sensor.pullup = Pullup::USE_EXTERN;
    sensor.init();
    sensor.enableInterrupts(doA, doB, doC);
    Serial.println("Sensor ready");

    // initialise the current sensing
    current_sense.init();
    motor.linkCurrentSense(&current_sense);


    // pwm frequency to be used [Hz]
    driver.pwm_frequency = 50000;
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



    _delay(1000);
}

void loop() {
    // IMPORTANT - call as frequently as possible
    // update the sensor values 
    sensor.update();
    // display the angle and the angular velocity to the terminal
    Serial.print(sensor.getAngle());
    Serial.print("\t");
    Serial.println(sensor.getVelocity());
}

#endif