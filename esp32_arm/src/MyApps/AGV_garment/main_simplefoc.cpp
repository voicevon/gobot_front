/**
 * 
 * Position/angle motion control example
 * Steps:
 * 1) Configure the motor and magnetic sensor  
 * 2) Run the code
 * 3) Set the target angle (in radians) from serial terminal
 * 
 */

#include <SimpleFOC.h>



// BLDC motor & driver instance  电机磁极对数设置，本店配套的无刷电机 磁极对数为 7 
// BLDCMotor motor = BLDCMotor(15);
// BLDCDriver3PWM driver = BLDCDriver3PWM(3, 5, 6, 7);

// angle set point variable
// float target_angle = 2;
// float target_velocity = 3.14*2;
// instantiate the commander
// Commander commander = Commander(Serial);
// PIDController pid = motor.P_angle;
// PIDController pid = motor.PID_velocity;
// void doTarget(char* cmd) { commander.scalar(&target_angle, cmd); }
// void doTarget(char* cmd) { commander.scalar(&target_velocity, cmd); }
// void onPid(char* cmd){ commander.pid(&pid,cmd); }

// HallSensor* sensor;// = HallSensor(21,22,23,24);

// void doA_left(){sensor->handleA();}
// void doB_left(){sensor->handleB();}
// void doC_left(){sensor->handleC();}



// void setup_simplefoc() {
// 	sensor->pullup = Pullup::USE_INTERN;
// 	// void* (*)(void *) cc;
// 	void (*foo)();
// 	void (HallSensor::* xx)() = & HallSensor::init; 
// 	HallSensor hhh = HallSensor(1,2,3,4);
// 	(hhh.*xx)();
// 	// foo= 	void (HallSensor::* xx)() = & HallSensor::init;
// 	// xx= &sensor->attachSectorCallback;
// 	sensor->enableInterrupts(foo, foo, doC_left);
// 	sensor->init();
// 	motor.linkSensor(sensor);

// 	driver.voltage_power_supply = 6;
// 	driver.voltage_limit = 6;
// 	driver.init();
// 	motor.linkDriver(&driver);

// 	// motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

// 	// set motion control loop to be used
// 	motor.controller = MotionControlType::angle;   //电机控制方式：位置闭环控制
// 	// motor.controller = MotionControlType::velocity;   //电机控制方式：位置闭环控制

// 	// contoller configuration 
// 	// default parameters in defaults.h

// 	//PID参数设置
// 	// velocity PI controller parameters
// 	motor.PID_velocity.P = 0.3;    
// 	motor.PID_velocity.I = 0.9;
// 	motor.PID_velocity.D = 0.0;
// 	// maximal voltage to be set to the motor
// 	motor.voltage_limit = 12;    //设置电机的电压极限
// 	motor.LPF_velocity.Tf = 0.3;     //低通滤波器的时间常数 （0~0.5）

// 	// angle P controller 
// 	motor.P_angle.P = 2.1;    // 角度P控制器
// 	motor.P_angle.output_ramp = 5;
// 	motor.velocity_limit = 100;    //设置转速极限

// 	// use monitoring with serial 
// 	Serial.begin(115200);
// 	// comment out if not needed
// 	motor.useMonitoring(Serial);


// 	// initialize motor
// 	motor.init();
// 	motor.initFOC();

// 	// add target command T
// 	commander.add('T', doTarget, "target angle");
// 	commander.add('C',onPid,"velocity pid");
// 	Serial.println("Motor ready.");
// 	Serial.println("Set the  Txx using serial terminal:");
// 	_delay(1000);
// }

// void loop_simplefoc() {

//   motor.loopFOC();

//   // Motion control function
//   // velocity, position or voltage (defined in motor.controller)
//   // this function can be run at much lower frequency than loopFOC() function
//   // You can also use motor.move() and set the motor.target in the code
//   motor.move(target_angle);
// //   motor.move(target_velocity);


//   // function intended to be used with serial plotter to monitor motor variables
//   // significantly slowing the execution down!!!!
//   motor.monitor();
  
//   // user communication
//   commander.run();
// }
