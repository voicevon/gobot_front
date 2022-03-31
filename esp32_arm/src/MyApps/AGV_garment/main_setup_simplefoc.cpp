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
HallSensor sensor = HallSensor(21,22,23,24);



// BLDC motor & driver instance  电机磁极对数设置，本店配套的无刷电机 磁极对数为 7 
BLDCMotor motor = BLDCMotor(15);

//无刷电机的3个PWM引脚和使能设置（PWM引脚1，PWM引脚2，PWM引脚3，使能引脚）
BLDCDriver3PWM driver = BLDCDriver3PWM(3, 5, 6, 7);

// angle set point variable
float target_angle = 2;
// float target_velocity = 3.14*2;
// instantiate the commander
Commander commander = Commander(Serial);
// PIDController pid = motor.P_angle;
PIDController pid = motor.PID_velocity;
void doTarget(char* cmd) { commander.scalar(&target_angle, cmd); }
// void doTarget(char* cmd) { commander.scalar(&target_velocity, cmd); }
void onPid(char* cmd){ commander.pid(&pid,cmd); }

void doA(){sensor.handleA();}
void doB(){sensor.handleB();}
void doC(){sensor.handleC();}



void setup_simplefoc() {

	// initialise magnetic sensor hardware 传感器初始化设置
	sensor.pullup = Pullup::USE_INTERN;
	sensor.init();
	// enable hall sensor hardware interrupts
	sensor.enableInterrupts(doA, doB, doC);
	// interrupt initialization
	// PciManager.registerListener(&listenA);
	// PciManager.registerListener(&listenB);
	// PciManager.registerListener(&listenC);
	// link the motor to the sensor 
	motor.linkSensor(&sensor);

	// driver config
	// power supply voltage [V]  设置提供的电源电压值
	driver.voltage_power_supply = 6;
	driver.voltage_limit = 6;
	driver.init();
	// link the motor and the driver
	motor.linkDriver(&driver);

	// choose FOC modulation (optional)
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

	// velocity low pass filtering time constant
	// the lower the less filtered
	motor.LPF_velocity.Tf = 0.3;     //低通滤波器的时间常数 （0~0.5）

	// angle P controller 
	motor.P_angle.P = 2.1;    // 角度P控制器
	motor.P_angle.output_ramp = 5;
	// maximal velocity of the position control
	motor.velocity_limit = 100;    //设置转速极限

	// use monitoring with serial 
	Serial.begin(115200);
	// comment out if not needed
	motor.useMonitoring(Serial);


	// initialize motor
	motor.init();
	// align sensor and start FOC
	motor.initFOC();

	// add target command T
	commander.add('T', doTarget, "target angle");
	// commander.add('T', doTarget, "target speed input here");
	commander.add('C',onPid,"velocity pid");
	Serial.println("Motor ready.");
	// Serial.println("Set the target angle using serial terminal:");
	Serial.println("Set the  Txx using serial terminal:");
	_delay(1000);
}

void loop_useless() {

  // main FOC algorithm function
  // the faster you run this function the better
  // Arduino UNO loop  ~1kHz
  // Bluepill loop ~10kHz 
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
