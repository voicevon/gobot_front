#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "mcp23018.h"
#include <Wire.h>
// #include <../Wire/Wire.h>  //this chip uses wire
#include <Wire.h>


mcp23018::mcp23018(){
	
}


mcp23018::mcp23018(const uint8_t adrs){
	postSetup(adrs);
}

void mcp23018::postSetup(const uint8_t adrs){
	if (adrs >= 0x20 && adrs <= 0x27){//HAEN works between 0x20...0x27
		_adrs = adrs;
		_error = false;
	} else {
		_error = true;
		Serial.print("\n\n\n !!!!! MCP23018 postSetup got error   !!! \n\n\n\n");
	}
	//setup register values for this chip
	// https://www.microchip.com/forums/m906742.aspx
	// We are working on BANK=0
	IODIR = 	0x00;
	IPOL = 		0x02;
	GPPU = 		0x0c;
	GPIO = 		0x12;


	// Below is not verified!!! They are in BANK=1
	//IOCON = 	0x05;  
	GPINTEN = 	0x02;
	DEFVAL = 	0x03;
	INTF = 		0x07;
	INTCAP = 	0x08;
	OLAT = 		0x0A;
	INTCON = 	0x04;
}

void mcp23018::begin(bool protocolInitOverride) {
	if (!protocolInitOverride && !_error){
		Wire.begin();
		#if ARDUINO >= 157
			Wire.setClock(400000UL); // Set I2C frequency to 400kHz
		#else
			TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
		#endif
	}	
	delay(100);
	//BANK=0, SEQ=1(disabled)
	// writeByte(IOCON,0b00100000);//read datasheet for details!  
	writeByte(0x5,0);
	writeByte(0xa,0);
	_gpioDirection = 0xFFFF;//all in
	_gpioState = 0x0000;//all low 
}



uint16_t mcp23018::readAddress(byte addr){
	byte low_byte = 0;
	byte high_byte = 0;
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)2);
		low_byte = Wire.read();
		high_byte = Wire.read();
	}	
	return byte2word(high_byte,low_byte);
}



void mcp23018::gpioPinMode(uint16_t mode){
	if (mode == INPUT){
		_gpioDirection = 0xFFFF;
	} else if (mode == OUTPUT){	
		_gpioDirection = 0x0000;
		_gpioState = 0x0000;
	} else {
		_gpioDirection = mode;
	}
	writeWord(IODIR,_gpioDirection);
}

void mcp23018::gpioPinMode(uint8_t pin, bool mode){
	if (pin < 16){//0...15
		mode == INPUT ? _gpioDirection |= (1 << pin) :_gpioDirection &= ~(1 << pin);
		writeWord(IODIR,_gpioDirection);
	}
}

void mcp23018::gpioPort(uint16_t value){
	if (value == HIGH){
		_gpioState = 0xFFFF;
	} else if (value == LOW){	
		_gpioState = 0x0000;
	} else {
		_gpioState = value;
	}
	writeWord(GPIO,_gpioState);
}

void mcp23018::gpioPort(byte lowByte, byte highByte){
	_gpioState = byte2word(highByte,lowByte);
	writeWord(GPIO,_gpioState);
}


uint16_t mcp23018::readGpioPort(){
	return readAddress(GPIO);
}

uint16_t mcp23018::readGpioPortFast(){
	return _gpioState;
}

int mcp23018::gpioDigitalReadFast(uint8_t pin){
	int temp = 0;
	if (pin < 16) temp = bitRead(_gpioState,pin);
	return temp;
}

void mcp23018::portPullup(uint16_t data) {
	if (data == HIGH){
		_gpioState = 0xFFFF;
	} else if (data == LOW){	
		_gpioState = 0x0000;
	} else {
		_gpioState = data;
	}
	writeWord(GPPU, _gpioState);
}



void mcp23018::gpioDigitalWrite(uint8_t pin, bool value){
	if (pin < 16){//0...15
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
		writeWord(GPIO,_gpioState);
	}
}

void mcp23018::gpioDigitalWriteFast(uint8_t pin, bool value){
	if (pin < 16){//0...15
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
	}
}

void mcp23018::gpioPortUpdate(){
	writeWord(GPIO,_gpioState);
}

int mcp23018::gpioDigitalRead(uint8_t pin){
	if (pin < 16) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

uint8_t mcp23018::gpioRegisterReadByte(byte reg){
  uint8_t data = 0;
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(reg);
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
		data = Wire.read();
	}
  return data;
}

uint16_t mcp23018::gpioRegisterReadWord(byte reg){
  uint16_t data = 0;
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(reg);
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)_adrs,(uint8_t)1);
		data = Wire.read();
		data += Wire.read() << 8;//Ironicster
	}
  return data;
}


void mcp23018::gpioRegisterWriteByte(byte reg,byte data){
	writeByte(reg,data);
}

void mcp23018::gpioRegisterWriteWord(byte reg,word data){
	writeWord(reg,data);
}

/* ------------------------------ Low Level ----------------*/

void mcp23018::writeByte(byte addr, byte data){
	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);
		Wire.write(data);
		uint8_t ret = Wire.endTransmission();
		if (ret !=0){
			Serial.print("        mcp23018::writeByte() return=");
			Serial.println(ret);
		}
	}
}

uint8_t mcp23018::writeWord(byte addr, uint16_t data){
	writeByte(addr, data & 0xff);
	writeByte(addr+1, data>>8);

	return 0;

	if (!_error){
		Wire.beginTransmission(_adrs);
		Wire.write(addr);
		//Wire.write(word2lowByte(data));
		//Wire.write(word2highByte(data));
		Wire.write(data >> 8);
		Wire.write(data & 0xFF);
		// Serial.print("\\n mcp23018::writeword()   ");
		// Serial.print(data>>8, HEX);
		// Serial.print("     ");
		// Serial.print(data & 0xFF,HEX) ;

		uint8_t ret = Wire.endTransmission();
		if (ret !=0){
			Serial.print("\nmcp23018::writeword error: ");
			Serial.print(ret);
		}
		return ret;
// 		 * Output   0 .. success
//  *          1 .. length to long for buffer
//  *          2 .. address send, NACK received
//  *          3 .. data send, NACK received
//  *          4 .. other twi error (lost bus arbitration, bus error, ..)
//  *          5 .. timeout
	}else{
		Serial.print("\n mcp23018::writeWord()  Inside Error, before invoking....\n ");
		return _error;
	}
}


uint8_t mcp23018::get_chip_addr(void){
	return _adrs;
}
