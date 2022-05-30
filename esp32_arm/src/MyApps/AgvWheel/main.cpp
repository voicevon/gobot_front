// #include "all_devices.h"
#include "all_applications.h"
#ifdef I_AM_AGV_WHEEL

#include <SimpleFOC.h>
#include <SoftwareSerial.h>
#include "main_hw.h"

// https://github.com/simplefoc/Arduino-FOC/blob/master/examples/hardware_specific_examples/DRV8302_driver/6pwm_example/encoder/full_control_serial/full_control_serial.ino
// https://docs.simplefoc.com/drv8302_example

SoftwareSerial mySerial; // RX, TX
Commander commander = Commander(mySerial);
float target_velocity = 2; // 2Rad/s ~ 20rpm
void onScalar(char* cmd){ commander.scalar(&target_velocity, cmd); }


//  BLDCMotor(int pp, (optional R))
//  - pp  - pole pair number
//  - R   - phase resistance value - optional
BLDCMotor motor = BLDCMotor(MOTOR_POLE_PAIRS, 2.8);
PIDController pid = motor.PID_velocity;
void onPid(char* cmd){ commander.pid(&pid,cmd); }



//  - hallA, hallB, hallC    - HallSensor A, B and C pins
//  - pp                     - pole pairs
HallSensor sensor = HallSensor(PIN_HALL_SENSOR_U, PIN_HALL_SENSOR_V, PIN_HALL_SENSOR_W, MOTOR_POLE_PAIRS);

// Interrupt routine initialization
// channel A and B callbacks
void doA() { sensor.handleA(); }
void doB() { sensor.handleB(); }
void doC() { sensor.handleC(); }

// current sensor
// shunt resistor value
// gain value
// pins phase A,B
// LowsideCurrentSense current_sense = LowsideCurrentSense(0.01, 10.0, PIN_CURRENT_SENSOR_A, PIN_CURRENT_SENSOR_B);

// BLDCDriver6PWM driver = BLDCDriver6PWM(PIN_PHASE_A_HIGH, PIN_PHASE_A_LOW,
//                                        PIN_PHASE_B_HIGH, PIN_PHASE_B_LOW,
//                                        PIN_PHASE_C_HIGH, PIN_PHASE_C_LOW,
//                                        PIN_DRV8302_ENABLE);

BLDCDriver3PWM driver = BLDCDriver3PWM(PIN_PHASE_A_HIGH, 
                                       PIN_PHASE_B_HIGH, 
                                       PIN_PHASE_C_HIGH, 
                                       PIN_DRV8302_ENABLE);

void setup_gpio(){
    // For output
    pinMode(PIN_DC_CAL, OUTPUT);
    digitalWrite(PIN_DC_CAL, LOW);
    pinMode(PIN_GAIN, OUTPUT);
    digitalWrite(PIN_GAIN, LOW);

    // for input
    pinMode(PIN_POWER_IS_GOOD, INPUT_PULLUP);
    pinMode(PIN_FAULT, INPUT_PULLUP);
    pinMode(PIN_OverCurrentTermperatureWarning, INPUT_PULLUP);

    mySerial.begin(115200, SWSERIAL_8N1, PIN_SERIAL_RX, PIN_SERIAL_TX,false);
    if (!mySerial) { // If the object did not initialize, then its configuration is invalid
        Serial.println("Invalid SoftwareSerial pin configuration, check config"); 
        while (1) { // Don't continue with invalid configuration
            delay (1000);
        }
    }
    mySerial.println("Hello, world?");
}

void setup_sensor(){
    sensor.pullup = Pullup::USE_INTERN;
    sensor.init();
    sensor.enableInterrupts(doA, doB, doC);
    Serial.println("Sensor ready");

    // initialise the current sensing
    // current_sense.init();
}

void setup_driver(){
    driver.pwm_frequency = 40000;
    driver.voltage_power_supply = 12;
    // Max DC voltage allowed - default voltage_power_supply
    driver.voltage_limit = 6;
    // daad_zone [0,1] - default 0.02 - 2%
    // driver.dead_zone = 0.05;
    int rrr = driver.init();
    Serial.print("driver.init() result =  ");
    Serial.println(rrr);
    driver.enable();
    // Serial.println("driver is initilized,  not enabled.");
    // delay(5000);

    // BLDCDriver3PWM xxx= BLDCDriver3PWM(15,16,16);
    // int xx=xxx.init();
    // Serial.print(xx);
    // Serial.println(xxx.initialized);
}

void setup_motor(){
        // controller configuration based on the control type
    // velocity PID controller parameters
    // default P=0.5 I = 10 D = 0
    motor.PID_velocity.P = 0.2;
    motor.PID_velocity.I = 20;
    motor.PID_velocity.D = 0.001;
    // jerk control using voltage voltage ramp
    // default value is 300 volts per sec  ~ 0.3V per millisecond
    motor.PID_velocity.output_ramp = 1000;
	motor.voltage_limit = 6;    //设置电机的电压极限

    // velocity low pass filtering
    // default 5ms - try different values to see what is the best.
    // the lower the less filtered
    motor.LPF_velocity.Tf = 0.01;

    // setting the limits
    // either voltage
    motor.voltage_limit = 10; // Volts - default driver.voltage_limit
    // of current
    motor.current_limit = 2; // Amps - default 0.2Amps
	motor.useMonitoring(Serial);
    motor.monitor_downsample = 10000;

	// initialize motor
	motor.init();
	// align sensor and start FOC
    Serial.println(" --------------------------------------- ");
	motor.initFOC();
}

void setup()
{
    Serial.begin(115200);
    setup_gpio();
    setup_sensor();
    motor.linkSensor(&sensor);
    setup_driver();
    // motor.linkCurrentSense(&current_sense);
    motor.linkDriver(&driver);

    // bool instance = driver;
    // Serial.print(" driver.initialized =  ");
    // Serial.println(motor.driver->initialized);
	motor.controller = MotionControlType::velocity;   //电机控制方式：位置闭环控制
    setup_motor();

    commander.add('T', onScalar, "target velocity");
	// commander.add('T', doTarget, "target speed input here");
	commander.add('C', onPid, "velocity pid");
    Serial.println("Hello , setup() is done...");
    _delay(1000);
    driver.enable();
}

int counter_high=0;
int counter_low =0;
void debug(){
    int power_good = digitalRead(PIN_POWER_IS_GOOD);
    if (power_good == HIGH)
        counter_high ++;
    else{
        counter_low++;
        Serial.print(counter_high);
        Serial.print("\t");
        Serial.print(counter_low);
        Serial.print("\t");
        Serial.print(float(counter_high) /  float(counter_low));
        Serial.print("\t\t");
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
}

void loop()
{
    sensor.update();
    motor.loopFOC();
    motor.move(target_velocity);

    motor.monitor();
    commander.run();

    // debug();

}

#endif