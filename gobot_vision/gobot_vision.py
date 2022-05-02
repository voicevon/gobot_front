

import sys
sys.path.append('D:\\XumingSource\\gobot_front')  # For runing in VsCode on Windows-10 

# from gogame.chessboard import ChessboardLayout
from config.config import Config as app_config
from gobot_vision.commander import Commander
from gobot_vision.commander_vision import CommanderVision
from gobot_vision.chessboard_scanner import ChessboardScanner, config_4_aruco_marks as chessboard_config
from gobot_vision.warehouse_vision import WarehouseVision
from vision.grid_finder import GridFinder

# from von.terminal_font import TerminalFont
import logging
from Pylib.image_logger import ImageLogger


import cv2
from Pylib.message_logger import MessageLogger




class GobotVision():

    def __init__(self):
        '''
        1. Overview of origin_image.
        2. Process:
           * Find all aruco marks, and there position
           * Get perspectived veiws of command_image, board_image, house_vendor_image
           * Scan the segmented images, To get command, board_layout, house_vender_stone.
        '''

        self.__chessboard_scanner = ChessboardScanner()
        config = self.__chessboard_scanner.get_4_aruco_marks_config()
        self.__chessboard_grid_finder = GridFinder(config)

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

        self.__publish_image = False
        logging.warn('Init vision is done......')

    def init_chessboard_layout(self):
        self.__chessboard_scanner.create_blank_layout()
        
    def get_stable_level (self, layout_history):
        stable_level = 0
        if layout_history[0][0][0] == layout_history[1][0][0]:
            stable_level += 1
        return stable_level

    def get_commander_plate_image(self, origin_image):
        '''
        Serve for Solution A, Not completed.
        '''
        commander_grid_image = self.__commander_grid_finder.detect_grid_from_aruco_corners()
        return commander_grid_image

    def get_command_index(self, origin_image):
        return self.__commander.get_command_from_image(origin_image)

    def get_chessboard_layout(self, origin_image):
        '''
        * Top level of getting layout.
        return 
        * layout, stable_depth. 
        * if stable_depth <= 0 , is saying can not get board image.
        '''
        perspective_image = self.__chessboard_grid_finder.detect_grid_from_aruco_corners(origin_image)
        if perspective_image is None:
            return None, -1
        
        # We got 4 corners. So we can do two (maybe 3 )things here
        # 1. Get pespectived image of chessboard
        # 2. Get house vender position(and house vender perspectived image)
        
        x0 = chessboard_config.crop_x0
        x1 = x0 + chessboard_config.crop_width
        y0 = chessboard_config.crop_y0
        y1 = y0 + chessboard_config.crop_height
        board_image = perspective_image[y0:y1, x0:x1]
        self.house_vendor_image = perspective_image[1:2,  3:4]

        if app_config.publish_image_board.value:
            ImageLogger.Output('gobot_image_board', perspective_image)
        if board_image is None:
            print('GobotVision.get_chessboard_layout()  Can NOT detect chessboard grid from origin_image')
            return None, 0

        layout, stable_depth = self.__chessboard_scanner.start_scan(board_image,3,True)
            #::wqlayout.print_out() 
        #print ('Stable Depth of the layout ', stable_depth)
        return (layout, stable_depth)

    def get_warehouse_plate(self,origin_image):
        config = WarehouseVision().create_finder_config()
        house_plate_finder = GridFinder(config)
        perspective_image = house_plate_finder.detect_grid_from_aruco_corners(origin_image)

        ImageLogger.Output("gobot_vision_perspective", perspective_image)

    def get_warehouse_stone_position(self, origin_image):
        '''
        Top level interface.
        '''
        return 1

    def get_commander_layout_for_solution_A_only(self, image, min_stable_depth=3, max_trying =5):
        '''
        Serve for Solution A, Not completed.
        '''
        history = []
        layout = self.__commander_vision.get_layout_from_image(image)
        history.append(layout)
        stable_count = self.get_stable_level(history)
        if stable_count >= min_stable_depth:
            return True,layout
        
        return False, -1

    def get_command_index_solution_A(self, origin_image):
        '''
        Deprecated solution.
        '''
        image = self.__commander_grid_finder.detect_grid_from_aruco_corners(origin_image)
        ret, layout = self.get_commander_layout_for_solution_A_only(image)
        if ret:
            for i in range(0,5,1):
                if layout[i] == BLACK_STONE:
                    return i
        return -1


if __name__ == '__main__':
    my_vision = GobotVision()
    history = []
    for i in range(0,6,1):
        layout = ChessboardScanner().create_blank_layout()
        history.append(layout)
    is_same = my_vision.is_same_layout(history)
    print(is_same)
    
