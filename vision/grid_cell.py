
import cv2
import numpy

import sys
sys.path.append('/home/pi/pylib')



class GridCell():
    def __init__(self, config):
        '''
        This is an abstract class.
        Children are:
            Go game -- stone
            Sower machine -- plate cell
        '''
        self.value = 0  # BLANK