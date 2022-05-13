# [GPIO resource = 25 Pins](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
* Left side = 11 Pins
* Right side = 12 Pins
* Bottom side = 3 Pins

# Analog Input x4 Pins or x2 Pins (or zero pins ) with ADS1115
* ADC (Voltage Sensor) x4
  * GPIO-36, GPIO-39,GPIO-34,GPIO-35

# PWM Output    x2 Pins
* PWM(for H-bridges) x2

# BUS           x12 Pins
* SPI for RFID Reader   x4
  * Better to use I2C , need to test.
  * GPIO-5, GPIO-18, GPIO-19, GPIO-23
* Serial Port           x2
  * GPIO-1, GPIO-3
* I2C 
* Dual I2C for APDS9960 x4 

# High speed    x5 Pins
* HCSR04         x2
* Stepper step   x2 
* LED (WS2812B)  x1

# Low speed     x11 Pins  or x2 (or x0) with MCP23018
* Input
  * Emergincy Stop    x1
  * Box_Carrier Home  x2

* Output
  * H-Bridges Dir     x2
  * Stepper Enable    x2
  * Stepper Dir       x2
  





