# [GPIO resource = 25 Pins](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
* Left side = 11 Pins
* Right side = 12 Pins
* Bottom side = 3 Pins

## Analog Input x2 Pins
* ADC (Voltage Sensor) x2
  * GPIO-36, GPIO-39,GPIO-34,GPIO-35


## BUS           x12 Pins
* SPI for RFID Reader   x4
  * Better to use I2C , need to test.
  * GPIO-5, GPIO-18, GPIO-19, GPIO-23
* Serial Port  for ISP programmer    x2
  * GPIO-1, GPIO-3
* I2C 
* Dual I2C for Bus
  * i2c_bus_a:
    * mcp_23018 + apds9960_left 
  * i2c_bus_b:
    * apds9960_right + 4 x VL53Lox

## High speed    x7 Pins
* Stepper step   x2
* H-Bridge(PWM)  x4 
* LED (WS2812B)  x1

## Low speed     x7 Pins  or x2 (or x0) with MCP23018
* Input
  * Emergincy Stop    x1
  * Box_Carrier Home  x2

* Output
  * Stepper Enable    x2
  * Stepper Dir       x2
  
 
# Hardware summry
* Esp32 + MCP23018 + 2x4988  + 2xHBridge
* 2x Hall sensor
* 2x APDS9960
* 4x VL53LoX

* 1X Rfid Reader
* 16x WS2812B




