#pragma once
#include <SimpleFOC.h>

/*
A wheel is composed with:
    one encoder
    one bldc motor
    one bldc driver
    lot of PID controllers.
*/

class SingleWheel_config{
	public:
		uint8_t hall_sensor_pin_a;
		uint8_t hall_sensor_pin_b;
		uint8_t hall_sensor_pin_c;
		uint8_t hall_sensor_pp = 15;

		void (*hall_do_A)();
		void (*hall_do_B)();
		void (*hall_do_C)();

		uint8_t driver_pin_a;
		uint8_t driver_pin_b;
		uint8_t driver_pin_c;
		uint8_t driver_pin_enable;
};


class SingleWheel_BLDC{
	public:
		SingleWheel_BLDC(SingleWheel_config config);
		HallSensor* sensor;  // = HallSensor() ;// = HallSensor(21,22,23,24);
		float target_angle = 2;
		PIDController pid = motor.PID_velocity;
		BLDCMotor motor = BLDCMotor(15);
		BLDCDriver3PWM* driver;  // = BLDCDriver3PWM();
		Commander commander = Commander(Serial);

		void loop_simplefoc();
		void Test(){
			// __config.do_A = 11;
		};
	private:
		SingleWheel_config __config;

};