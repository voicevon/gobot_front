
# from typing_extensions import runtime_checkable
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
from vision.aruco_finder import ArucoFinder
from vision.grid_plate import GridPlate
from vision.grid_cell import GridCell



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


class gridplate_config()
    pass

class WarehouseRobot():

    def __init__(self):
        self.__eye = MonoEye()
        self.__finder = ArucoFinder(area_size=(200,600), mark_ids= [1,2,3,4], enable_mqtt=True)
        # self.__scanner = StoneScanner()

        self.__grid_plate = GridPlate(gridplate_config)

        self.__cell_scanner = CellScanner()
        self.__plane_motor = Stepper()

        self.__target_x_position = 100
        self.__target_y_position = 30
    
    def get_cell_image(self, x, y, perspect_image):
        
        return perspect_image

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
        plate_image = self.__finder.auto_perspect(origin_image)
        if plate_image is None:
            return 
        layout = self.__grid_plate.scan_layout(plate_image, history_length=1, show_processing_image=False, pause_second=0)
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
    g_mqtt.connect_to_broker('123456', 'voicevon.vicp.io', 1883, 'von','von1970')

    myrobot = WarehouseRobot()
    while True:
        myrobot.spin_once()
        time.sleep(0.01)
        print('spin_once done...')
