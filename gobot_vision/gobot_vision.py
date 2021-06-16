from gobot_vision.commander_vision import CommanderVision
from gobot_vision.commander import Commander
from vision.grid_finder import GridFinder
from gobot_vision.chessboard_vision import BLACK_STONE, ChessboardVision, config_4_aruco_marks
from gobot_vision.warehouse_vision import WarehouseVision
import cv2
import numpy as np

import sys
sys.path.append('/home/pi/pylib')
from terminal_font import TerminalFont
from mqtt_helper import g_mqtt


class GobotVision():
    def __init__(self):
        '''
        '''
        self.__chessboard_vision = ChessboardVision()
        config = self.__chessboard_vision.get_4_aruco_marks_config()
        self.__chesboard_grid_finder = GridFinder(config)

        self.__commander_solution = 2
        if self.__commander_solution == 1:
            # solution A
            # unified to chessboard_vision, might be slower
            self.__commander_vision = CommanderVision()
            config = self.__commander_vision.create_2_aruco_marks_config()
            self.__commander_grid_finder = GridFinder(config)

        if self.__commander_solution == 2:
            # solution B
            # Simpler solution, faster, might be less stable
            self.__commander = Commander()



    def get_stable_level (self, layout_history):
        stable_level = 0
        if layout_history[0][0][0] == layout_history[1][0][0]:
            stable_level += 1
        return stable_level
        
    

    def get_commander_plate_image(self, origin_image):
        commander_grid_image = self.__commander_grid_finder.detect_grid_from_aruco_corners()
        return commander_grid_image

    def get_commander_layout(self, image, min_stable_depth=3, max_trying =5):
        history = []
        layout = self.__commander_vision.get_layout_from_image(image)
        history.append(layout)
        stable_count = self.get_stable_level(history)
        if stable_count >= min_stable_depth:
            return True,layout
        
        return False, -1

    def get_command_index(self, origin_image):
        if self.__commander_solution == 1:
            image = self.__commander_grid_finder.detect_grid_from_aruco_corners(origin_image)
            ret, layout = self.get_commander_layout(image)
            if ret:
                for i in range(0,5,1):
                    if layout[i] == BLACK_STONE:
                        return i
            return -1
        if self.__commander_solution == 2:
            return self.__commander.get_command_from_image(origin_image)


    def get_chessboard_plate_image(self, origin_image):
        board_image = self.__chesboard_grid_finder.detect_grid_from_aruco_corners(origin_image)
        return board_image

    def get_chessboard_layout(self, image, min_stable_depth=3, max_trying = 6):
        history = []
        layout = self.__chessboard_vision.get_layout_from_image(image)
        # history.append(layout)
        # stable_count = self.get_stable_level(history)
        # if stable_count >= min_stable_depth:
        #     return True,layout
        
        return False, -1
            

    def get_warehouse_plate(self):
        return 1

    def get_warehouse_stone_position(self):
        return 1


if __name__ == '__main__':
    my_vision = GobotVision()
    history = []
    for i in range(0,6,1):
        layout = ChessboardVision().create_blank_layout()
        history.append(layout)
    is_same = my_vision.is_same_layout(history)
    print(is_same)
    