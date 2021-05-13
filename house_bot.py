
# from typing_extensions import runtime_checkable
import cv2
import numpy as np
from math import sin, cos
from picamera.array import PiRGBArray
from picamera import PiCamera
import time

import sys
sys.path.append("~/pylib")
# sys.path.append("../pylib")
from terminal_font import TerminalFont
from mqtt_helper import g_mqtt
from house_bot import Stepper
from scanner import PiEye, Scanner


class WarehouseRobot():

    def __init__(self):
        self.__eye = PiEye()
        self.__scanner = Scanner("warehouse")
        self.__plane_motor = Stepper()

        self.__target_x_position = 100
        self.__target_y_position = 30

    def spin_once(self):
        # Take a picture from camera
        image = self.__eye.take_picture()
        g_mqtt.publish_cv_image('gobot_stonehouse/eye/origin', image)

        # Get corners position from detecting aruco marks
        # The sequerence is always [TopLeft, TopRight,bottomRight,BottomLeft]
        corners = self.__eye.find_corners(image,[1,2,4,3])
        print(corners)
        if corners != None:
            if len(corners) == 4:
                # Get perspectived image
                perspect_img = self.__eye.get_perspective_view(image,corners)
                g_mqtt.publish_cv_image('gobot_stonehouse/eye/perspect', perspect_img)
        
                # Get the stone position, will store the position to where? 
                x, y = self.__scanner.get_stone_postion(perspect_img, 'BLACK') 
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
