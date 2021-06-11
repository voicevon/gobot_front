


# import cv2
# import numpy as np
# from math import sin, cos
# from picamera.array import PiRGBArray
# from picamera import PiCamera

import sys
sys.path.append("/home/pi/pylib")
from terminal_font import TerminalFont
from mqtt_helper import g_mqtt

from config import config
from vision.grid_helper import GridHelper
from house_motor import Stepper
from vision.robot_eye import MonoEye





class finder_config:
    aruco_ids = [1, 2, 3, 4]  # [topleft, topright, bottomright, bottomleft]
    area_scales = [1.1, 1.1, 2.2, 2.2]
    area_size = (200,900)    # for pespectived view image.

class layout_config:
    '''
    All the cells has its own logical value.
    Not involved to phisical size or area.

    y (90)
    ^
    |
    |
    +---------> x (60)
    '''

    name = 'house_grid_plate'
    rows = 90
    cols = 60



class stone_config:
    '''
    '''
    class value:
        blank = 1
        black = 2
        white = 3

class WarehouseRobot():

    def __init__(self):
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
        self.__eye = MonoEye()
        self.__grid_helper = GridHelper(finder_config, stone_config, layout_config)
        self.__plane_motor = Stepper()
        self.__target_x_position = 100
        self.__target_y_position = 30
        self.__counter = 0
    
    def get_first_stone_postion(self, layout):
        '''
        The stone that most closed to target position
        '''
        for y in range(0,10):
            for x in range(0,10):
                pass
                # cell_image = self.get_cell_image(image, x, y)
                
                # color = self.__cell_scanner.scan(cell_image)
                #if color = self.CellScanner.COLOR_BLACK:
                #    return x, y
        return None, None

    def spin_once(self):
        print('spin once...', self.__counter)
        self.__counter += 1
        origin_image = self.__eye.take_picture()
        # return

        if config.publish_mqtt:
            g_mqtt.publish_cv_image('gobot_stonehouse/eye/origin', origin_image)
        # return

        perspect_image = self.__grid_helper.grid_finder.auto_perspect(origin_image)
        if perspect_image is None:
                return
        layout, stable_depth = self.__grid_helper.scan_layout(perspect_image, history_length=1, show_processing_image=False, pause_second=0)
        if layout is None:
            return
        layout.print_out()
        x, y = self.get_first_stone_postion(layout)
        if x is None:
            return

        print('Linear motor start to move a stone')
        # How far is the stone to the target position?
        dx = self.__target_x_position - x
        dy = self.__target_y_position - y

        # Control the plane motor to drive the stone move
        self.__plane_motor.move_stone(dx, dy)
        print('---------------------------------------')
        



import time
  

if __name__ == '__main__':
    config.publish_mqtt = True
    if config.publish_mqtt:
        g_mqtt.connect_to_broker('123456', 'voicevon.vicp.io', 1883, 'von','von1970')

    myrobot = WarehouseRobot()
    while True:
        myrobot.spin_once()
        time.sleep(0.01)
        print('spin_once done...')
