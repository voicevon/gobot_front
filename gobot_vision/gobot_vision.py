from gobot_vision.commander_vision import CommanderVision
from vision.grid_finder import GridFinder
from gobot_vision.chessboard_vision import ChessboardVision, config_4_aruco_marks
from gobot_vision.warehouse_vision import WarehouseVision
import cv2
import numpy as np


class GobotVision():
    def __init__(self):
        '''
        '''
        self.__chessboard_vision = ChessboardVision()
        config = self.__chessboard_vision.get_4_aruco_marks_config()
        self.__chesboard_grid_finder = GridFinder(config)

        self.__commander_vision = CommanderVision()
        config = self.__commander_vision.create_2_aruco_marks_config()
        self.__commander_grid_finder = GridFinder(config)



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



    def get_chessboard_plate_image(self, origin_image):
        board_image = self.__chesboard_grid_finder.detect_grid_from_aruco_corners(origin_image)
        return board_image

    def get_chessboard_layout(self, image, min_stable_depth=3, max_trying = 6):
        history = []
        layout = self.__chessboard_vision.get_layout_from_image(image)
        history.append(layout)
        stable_count = self.get_stable_level(history)
        if stable_count >= min_stable_depth:
            return True,layout
        
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
    
