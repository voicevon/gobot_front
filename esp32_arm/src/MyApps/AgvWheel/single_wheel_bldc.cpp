#include "single_wheel_bldc.h"

SingleWheel_BLDC::SingleWheel_BLDC(SingleWheel_config config){
    this->config = config;
}

void SingleWheel_BLDC::Init(){
    // this->__sensor = new HallSensor(config.hall_sensor_pin_a, config.hall_sensor_pin_b, config.driver_pin_c, config.hall_sensor_pp);
	this->__sensor->pullup = Pullup::USE_INTERN;
	// void* (*)(void *) cc;
	// void (*foo)();
	// void (Hall__sensor::* xx)() = & Hall__sensor::init; 
	// Hall__sensor hhh = Hall__sensor(1,2,3,4);
	// (hhh.*xx)();
	// foo= 	void (Hall__sensor::* xx)() = & Hall__sensor::init;
	// xx= &__sensor->attachSectorCallback;
	this->__sensor->init();
	this->__sensor->enableInterrupts(config.hall_do_A, config.hall_do_B, config.hall_do_C);
	this->motor.linkSensor(this->__sensor);

    this->__driver = new BLDCDriver3PWM(config.driver_pin_a, config.driver_pin_b, config.driver_pin_c, config.driver_pin_enable);
	this->__driver->voltage_power_supply = 6;
	this->__driver->voltage_limit = 6;
	this->__driver->init();
	this->motor.linkDriver(this->__driver);

	// motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

	// set motion control loop to be used
	motor.controller = MotionControlType::angle;   //电机控制方式：位置闭环控制
	// motor.controller = MotionControlType::velocity;   //电机控制方式：位置闭环控制

	// contoller configuration 
	// default parameters in defaults.h

	//PID参数设置
	// velocity PI controller parameters
	motor.PID_velocity.P = 0.3;    
	motor.PID_velocity.I = 0.9;
	motor.PID_velocity.D = 0.0;
	// maximal voltage to be set to the motor
	motor.voltage_limit = 12;    //设置电机的电压极限
	motor.LPF_velocity.Tf = 0.3;     //低通滤波器的时间常数 （0~0.5）

	// angle P controller 
	motor.P_angle.P = 2.1;    // 角度P控制器
	motor.P_angle.output_ramp = 5;
	motor.velocity_limit = 100;    //设置转速极限

	// use monitoring with serial 
	Serial.begin(115200);
	// comment out if not needed
	motor.useMonitoring(Serial);


	// initialize motor
	motor.init();
	motor.initFOC();

	// add target command T
	// commander.add('T', doTarget, "target angle");
	// commander.add('C',onPid,"velocity pid");
	Serial.println("Motor ready.");
	Serial.println("Set the  Txx using serial terminal:");
	_delay(1000);
}

void SingleWheel_BLDC::SpinOnce() {
    motor.loopFOC();
    // Motion control function
    // velocity, position or voltage (defined in motor.controller)
    // this function can be run at much lower frequency than loopFOC() function
    // You can also use motor.move() and set the motor.target in the code
    motor.move(target_angle);
    //   motor.move(target_velocity);
    // function intended to be used with serial plotter to monitor motor variables
    // significantly slowing the execution down!!!!
    motor.monitor();
    // user communication
    commander.run();
}