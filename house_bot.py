
# from typing_extensions import runtime_checkable
from config import config
from vision.grid_helper import Grid_helper
import cv2
import numpy as np
from math import sin, cos
from picamera.array import PiRGBArray
from picamera import PiCamera
import time

import sys
sys.path.append("/home/pi/pylib")
# sys.path.append("../pylib")
from terminal_font import TerminalFont
from mqtt_helper import g_mqtt
from house_motor import Stepper
from vision.robot_eye import MonoEye
from vision.grid_helper import Grid_Helper
# from vision.grid_finder import GridFinder
# from vision.grid_plate import GridPlate
# from vision.grid_cell import GridCell




class StoneScanner():

    def __init__(self):
        pass

    def get_first_stone_postion(self, img, color):
        '''
                 y
                 ^
                 |
                 |
        ---------+---------> x
        '''
        color = 'BLACK'
        for y in range(50,0,-1):
            for x in range (-50,50):
                if True:
                    # Got black stone
                    return x, y
        return None,None

class finder_config:
    aruco_ids = [1, 2, 3, 4]  # [topleft, topright, bottomright, bottomleft]
    area_scales = [1.1, 1.1, 2.2, 2.2]
    real_size = (900,600)    # for pespectived view image.

class grid_config:
    name = 'house_grid_plate'
    ROWS = 90
    COLS = 60


class stone_config:
    class value:
        blank = 1
        black = 2
        white = 3

class WarehouseRobot():
    '''
    This robot has a mono_eye, 
        Can take picture
    The robot also has a GridHelper()
        Can give us a perspective view from origin picture.
        Then convert a logical layout.
    Another part is layout_helper
        Can detect the first stone position.
    The target position, can be known from mqtt
    The robot calculate the path from stone position to target position
        the motor driver will move the stone to target position 
    '''
    def __init__(self):
        self.__eye = MonoEye()
        self.__grid_helper = Grid_Helper(finder_config, grid_config, stone_config)
        self.__plane_motor = Stepper()
        self.__target_x_position = 100
        self.__target_y_position = 30
    
    def get_first_stone_postion(self, layout):
        for y in range(0,10):
            for x in range(0,10):
                pass
                # cell_image = self.get_cell_image(image, x, y)
                
                # color = self.__cell_scanner.scan(cell_image)
                #if color = self.CellScanner.COLOR_BLACK:
                #    return x, y
        return None, None

    def spin_once(self):
        origin_image = self.__eye.take_picture()
        g_mqtt.publish_cv_image('gobot_stonehouse/eye/origin', origin_image)
        layout = self.__grid_helper.scan_layout(origin_image, history_length=1, show_processing_image=False, pause_second=0)
        if layout is None:
            return
        x, y = self.get_first_stone_postion(self.__grid_plate.last_layout)
        if x is None:
            return
        # How far is the stone to the target position?
        dx = self.__target_x_position - x
        dy = self.__target_y_position - y

        # Control the plane motor to drive the stone move
        self.__plane_motor.move_stone(dx, dy)
        print('---------------------------------------')
        
        

if __name__ == '__main__':
    if config.publish_mqtt:
        g_mqtt.connect_to_broker('123456', 'voicevon.vicp.io', 1883, 'von','von1970')

    myrobot = WarehouseRobot()
    while True:
        myrobot.spin_once()
        time.sleep(0.01)
        print('spin_once done...')
