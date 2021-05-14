import cv2
import numpy as np
#import threading
#import time  # only for sleep


from gogame.chessboard import ChessboardLayout, DiedAreaScanner
# from config import config, CvDebugger
# from terminal_font import TerminalFont


#from mark_scanner import MarkScanner
#from board_scanner import BoardScanner
#from layout_scanner import LayoutScanner
from vision.robot_eye import MonoEye
from vision.scanner import ArucoFinder

import sys
sys.path.append('/home/pi/pylib')
from terminal_font import TerminalFont

class AiClient():
    pass


class GobotHead():

    def __init__(self):
        self.__eye = MonoEye()
        self.__command_finder = ArucoFinder([345])
        self.__chessboard_finder = ArucoFinder([22,33])
        #self.__layout_scanner = AruScanner()
        self.__ai_client = AiClient()
        #self.__mark_scanner = MarkScanner()
        #self.__board_scanner = BoardScanner()
        #self.__layout_scanner = LayoutScanner()
        # self.__capture_device = cv2.VideoCapture(app.robot_eye.camera_index)

        # self.windows={'original':'original','candy':'candy','chessboard':'chessboard'}
        # self.__cvWindow = CvWindows()
        # self.__thread_eyes = {}

        self.__FC_YELLOW = TerminalFont.Color.Fore.yellow
        self.__FC_RESET = TerminalFont.Color.Control.reset
        self.__MARK_STABLE_DEPTH = 5
        self.__LAYOUT_STABLE_DEPTH = 5


    def get_stable_layout(self,min_stable_depth):
        stable_depth = 0
        while stable_depth < min_stable_depth:
            ret, img_origin = self.__capture_newest_image()
            if ret:
                # got origin image
                # img_board = self.__board_scanner.get_whole_area_of_chessboard(img_origin)
                img_board = self.__board_scanner.get_whole_area_of_chessboard_new_idea(img_origin)
                if img_board is not None:
                    # got board image
                    layout, stable_depth = self.__layout_scanner.start_scan(img_board, self.__LAYOUT_STABLE_DEPTH)
        layout.rename_to('stable detect layout (depth = %d)' % stable_depth)
        return layout


    def get_chessboard_test(self):
        ret, img = self.__capture_newest_image()
        if ret:
            self.new_idea(img)

    def get_stable_mark(self,min_stable_depth):
        stable_depth = 0
        while stable_depth < min_stable_depth:
            ret, img = self.__capture_device.read()
            if ret:
                mark_index, stable_depth = self.__mark_scanner.detect_mark(img, min_stable_depth)
        return mark_index
        
    def start_show(self,eye_name):
        xx = self.__cvWindow.get_all_windows()[eye_name]
        xx[0] = True
        # self.__thread_eye = threading.Thread(target=self.__robot_eye.monitor)
        if self.__thread_eyes.get(eye_name) == None:
            self.__thread_eyes[eye_name] = threading.Thread(target=self.__start_show, args=[eye_name])
            self.__thread_eyes[eye_name].start()
            
    def stop_show(self, eye_name):
        # this_window = self.__cvWindow.get_window(window_name)
        self.__cvWindow.get_all_windows()[eye_name][0] = False


    def spin(self):
        oringin_image = self.__eye.take_picture()
        command_image = self.__command_finder.auto_perspect(oringin_image)
        command = self.__commad_scanner.get_command(command_image)

        if user_is_playing:
            # scan chessboard
            if user_dropped:
                # send step to game server
                self.go_agent.send("move c4 B")
                
        



if __name__ == '__main__':
    myrobot = GobotHead()
    myrobot.spin()


    while True:
        menu = []
        menu.append('***********************************************************')
        menu.append('* 1. (TODO) List cameras, You can see the IDs.            *')
        menu.append('* 2. Capture image from camera (id = 0)                   *')
        menu.append('* 3.                                           *')
        menu.append('* 4. get_stable_layout()                                  *')
        menu.append('* 5. Scan instruction marks(mode=?)                       *')
        menu.append('* 6. get_stable_mark()                                    *')
        menu.append('* 7. Died_area_scanner                                    *')
        menu.append('***********************************************************')

