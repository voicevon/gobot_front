
import cv2
import numpy

import sys
sys.path.append('/home/pi/pylib')



class GridCell():
    def __init__(self, config):
        self.value = 0  # BLANK