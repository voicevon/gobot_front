/** I2C_Slave_Transmitter.ino
 *
 * | Arduino | A4 | ---- SDA
 *           | A5 | ---- SCL
 */

#include "all_application.h"

#include "touch_pad_channel.h"
#include <Wire.h>   // https://www.jianshu.com/p/4b1ddefc9006
#include <avr/wdt.h>

#ifdef I_AM_ACUPUNCTURE_MEGA328_VER_2023

#define TOUCH_PAD_SENSORS_COUNT 14


TouchpadChannel all_channels[TOUCH_PAD_SENSORS_COUNT];
uint8_t tx_buffer_for_i2c[TOUCH_PAD_SENSORS_COUNT];

void requestEvent(){
	Wire.write(tx_buffer_for_i2c, TOUCH_PAD_SENSORS_COUNT);
	Serial.print("replied");
}

void InitChannels(){
	int pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 16, 17, 18, 19}; // 14:A0, 15:A1, 16:A2, 17:A3, 18:A4, 19:A5
	for (int i = 0; i < TOUCH_PAD_SENSORS_COUNT; i++){
		all_channels[i].Init(pins[i]);
	}
}


void forceSystemReset() {
	MCUSR = 0;
	wdt_enable(WDTO_250MS);
	while (1)  ;  // force WDT to reset system
}

void setup() {
	Serial.begin(115200);
	Serial.println("  Hi boys and girl, be happy!");

	Wire.begin(MY_I2C_ADDR); // join I2C bus as slave (address provided)
	// Wire.setClock();
	Wire.onRequest(requestEvent); // register event

	InitChannels();
    // WatchDog::init(forceSystemReset, 500);
	// WatchDog::setPeriod(OVF_1000MS);
	// WatchDog::start();
	wdt_enable(WDTO_120MS);

	Serial.println("Setup is finished.");
}

void loop() {
	for (int i = 0; i < TOUCH_PAD_SENSORS_COUNT; i++){
		TouchpadChannel* channel = &all_channels[i];
		channel->Read();
		// prepare tx_buffer_for_i2c_request
		tx_buffer_for_i2c[i] = channel->GetValue();
		// delay(100);
		wdt_reset();
	}

	// WatchDog::start();

}

#endif