#include "board_agv_2205.h"

void Board2205Agv::Init(bool is_on_reset){
    Serial.println("[Error] Board2205Agv::Init()");
}

void Board2205Agv::Init(TwoWire* i2c_bus_main, TwoWire* i2c_bus_ext){
    // Convert Dual apds9960 on dual i2c bus, to one track sensor
    this->_Begin_Apds9960(&this->__left_aps9960, I2C_ADDR_APDS9960_2205, i2c_bus_main);
    this->_Begin_Apds9960(&this->__right_aps9960, I2C_ADDR_APDS9960_2205, i2c_bus_ext);
    this->__dual_9960.Init(&this->__left_aps9960, &this->__right_aps9960);

    // Convert vl53l0x chip to obstacle sensor
    this->_Begin_Vl531l0x(&this->__vl53l0x, I2C_ADDR_VL53L0X_2205, i2c_bus_ext);
    this->__obstacle_vl53l0x.Init(&this->__vl53l0x);


    // Dual-wheel pwm dc motor Driver
    this->__dual_wheel.LinkLeftDriver(&this->__left_wheel);
    this->__dual_wheel.LinkRightDriver(&this->__right_wheel);

    this->__neo_pixel.begin();
    this->__track_light.LinkDriver(&this->__neo_pixel);

    //init reader driver, then link the driver to functional reader.
	SPI.begin(PIN_RFID_SPI_CLK_2205, PIN_RFID_SPI_MISO_2205,PIN_RFID_SPI_MOSI_2205,PIN_RFID_SS_2205);			// Init SPI bus
	this->__mfrc522.PCD_Init();		// Init MFRC522
	// delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	this->__mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
    this->__rfid_reader.LinkDriver(&this->__mfrc522);


}
