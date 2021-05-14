
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
from vision.scanner import ArucoFinder, ArucoFinder


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


class WarehouseRobot():

    def __init__(self):
        self.__eye = MonoEye()
        self.__finder = ArucoFinder([1,2,3,4], enable_mqtt=True)
        self.__scanner = StoneScanner()
        self.__plane_motor = Stepper()

        self.__target_x_position = 100
        self.__target_y_position = 30
    

    def spin_once(self):
        origin_image = self.__eye.take_picture()
        g_mqtt.publish_cv_image('gobot_stonehouse/eye/origin', origin_image)
        board_image = self.__finder.auto_perspect(origin_image)
        if board_image != None:
            x, y = self.__scanner.get_first_stone_postion(board_image,'BLACK')
            if x != None:
                # How far is the stone to the target position?
                dx = self.__target_x_position - x
                dy = self.__target_y_position - y 

                # Control the plane motor to drive the stone move
                self.__plane_motor.move_stone(dx, dy)
        
        

if __name__ == '__main__':
    g_mqtt.connect_to_broker('123456', 'voicevon.vicp.io', 1883, 'von','von1970')

    myrobot = WarehouseRobot()
    while True:
        myrobot.spin_once()
        time.sleep(0.01)
        print('spin_once done...')
