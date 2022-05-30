// #include "all_devices.h"
#include "all_applications.h"
#ifdef I_AM_VL53L0X_SINGLE
#include "Adafruit_VL53L0X.h"




// #define PIN_SCL 26
// #define PIN_SDA 27

// #define PIN_SCL 22
// #define PIN_SDA 21


#define PIN_SDA 16
#define PIN_SCL 17 

Adafruit_VL53L0X* lox; //= Adafruit_VL53L0X();
// Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
	Serial.begin(115200);
	// wait until serial port opens for native USB devices
	while (! Serial) {
		delay(1);
	}
	TwoWire* bus = new TwoWire(1);
	bus->begin(PIN_SDA ,PIN_SCL);
	// Will update the chip to new address, and stop here. 
	// #define NEW_I2C_ADDRESS  0x29
	lox = new Adafruit_VL53L0X();
	#ifdef NEW_I2C_ADDRESS
		lox.begin(NEW_I2C_ADDRESS);
		Serial.println("\n\n\n\n Address is updated, please reboot, verify it with  i2c_scanner");
		while (true);
	#endif


	Serial.println("Adafruit VL53L0X test");
	if (!lox->begin(0x29, true, bus)) {
	// if (!lox.begin(0x29, true, bus)) {
		Serial.println(F("Failed to boot VL53L0X"));
		while(1);
	}
	Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
	VL53L0X_RangingMeasurementData_t measure;
		
	Serial.print("Reading a measurement... ");
	lox->rangingTest(&measure, false); // pass in 'true' to get debug data printout!
	// lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

	if (measure.RangeStatus != 4) {  // phase failures have incorrect data
		Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
	} else {
		Serial.println(" out of range ");
	}
	delay(100);
}


#endif