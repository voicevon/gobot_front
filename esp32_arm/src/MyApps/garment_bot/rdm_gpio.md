# [GPIO resource = 25 Pins]((https://electropeak.com/learn/full-guide-to-esp32-pinout-reference-what-gpio-pins-should-we-use/))
* Left side = 11 Pins
* Right side = 12 Pins
* Bottom side = 3 Pins

# Analog Input x2 Pins
* ADC (Voltage Sensor) x2
-* GPIO-36, GPIO-39

# PWM Output    x2 Pins
* PWM(for H-bridges) x2

# BUS           x12 Pins
* SPI for RFID Reader   x4
-* GPIO-5, GPIO-18, GPIO-19, GPIO-23
* Serial Port           x2
-* GPIO-1, GPIO-3
* Dual I2C for APDS9960 x4 

# High speed    x4 Pins
* HCSR04       x2
* Stepper step x2 

# Low speed     x11 Pins  or x2 (or x0) with MCP23018
* Input
-* Emergincy Stop    x1
-* Box_Carrier Home  x2

* Output
-* H-Bridges Dir     x2
-* Stepper Enable    x2
-* Stepper DIr       x2
-* LED               X2





