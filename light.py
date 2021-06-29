import board
import neopixel  #pip3 install rpi_ws281x adafruit-circuitpython-neopixel

class Light:

    self.__init__(self):
        pixels = neopixel.NeoPixel(board.D18, 30)
