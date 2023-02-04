/** I2C_Slave_Transmitter.ino
 *
 * | Arduino | A4 | ---- SDA
 *           | A5 | ---- SCL
 */

#include "all_application.h"

#include "touch_pad_channel.h"
#include <Wire.h>   // https://www.jianshu.com/p/4b1ddefc9006

#ifdef I_AM_ACUPUNCTURE_MEGA328_VER_2023

#define TOUCH_PAD_CHANNELS 14
#define MY_I2C_ADDR 11

TouchpadChannel all_channels[TOUCH_PAD_CHANNELS];
uint8_t tx_buffer_for_i2c[TOUCH_PAD_CHANNELS];

void requestEvent(){
	Wire.write(tx_buffer_for_i2c, TOUCH_PAD_CHANNELS);
	Serial.print("replied");
}

void InitChannels(){
	int pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 16, 17, 18, 19}; // 14:A0, 15:A1, 16:A2, 17:A3, 18:A4, 19:A5
	for (int i = 0; i < TOUCH_PAD_CHANNELS; i++){
		all_channels[i].Init(pins[i]);
	}
}

void setup() {
	Serial.begin(115200);
	Serial.println("  Hi boys and girl, be happy!");

	Wire.begin(MY_I2C_ADDR); // join I2C bus as slave (address provided)
	// Wire.setClock();
	Wire.onRequest(requestEvent); // register event

	InitChannels();
	Serial.println("Setup is finished.");
}

void loop() {
	for (int i = 0; i < TOUCH_PAD_CHANNELS; i++){
		TouchpadChannel* channel = &all_channels[i];
		channel->Read();
		// prepare tx_buffer_for_i2c_request
		tx_buffer_for_i2c[i] = channel->GetValue();
		delay(100);
	}
}

#endif