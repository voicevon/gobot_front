import board
import neopixel  #pip3 install rpi_ws281x adafruit-circuitpython-neopixel


# https://www.youtube.com/watch?v=Pxt9sGTsvFk&t=150s

class Light:

    self.__init__(self):
        pixels = neopixel.NeoPixel(board.D18, 30)
