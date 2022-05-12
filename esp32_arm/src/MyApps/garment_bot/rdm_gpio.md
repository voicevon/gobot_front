# [GPIO resource](https://electropeak.com/learn/full-guide-to-esp32-pinout-reference-what-gpio-pins-should-we-use/) = 25 Pins
* Left side = 11 Pins
* Right side = 12 Pins
* Bottom side = 3 Pins

# GPIO  Analog Input x2
* ADC (Voltage Sensor) x2
** GPIO-36, GPIO-39

# GPIO PWM Output    x2
* PWM(for H-bridges) x2

# GPIO BUS           x12
* SPI for RFID Reader   x4
** GPIO-5, GPIO-18, GPIO-19, GPIO-23
* Serial Port           x2
** GPIO-1, GPIO-3
* Dual I2C for APDS9960 x4 

# GPIO High speed    x4
* HCSR04       x2
* Stepper step x2 

# GPIO Low speed     x9  or x2 (or x0) (with MCP23018)
* Input
** Emergincy Stop    x1
* Output
** H-Bridges Dir     x2
** Stepper Enable    x2
** Stepper DIr       x2
** LED               X2





