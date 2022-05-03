

import sys
sys.path.append('D:\\XumingSource\\gobot_front')  # For runing in VsCode on Windows-10 

# from gogame.chessboard import ChessboardLayout
from config.config import Config as app_config
from gobot_vision.commander import Commander
from gobot_vision.commander_vision import CommanderVision
from gobot_vision.chessboard_scanner import ChessboardScanner, config_4_aruco_marks as chessboard_config
from gobot_vision.warehouse_vision import WarehouseVision
from vision.grid_finder import GridFinder   # TODO: remove this

# from von.terminal_font import TerminalFont
import logging
from Pylib.image_logger import ImageLogger, ImageLoggerToWhere
from Pylib.message_logger import MessageLogger
from vision.perspective_transfomer import PerspectiveTransformer
from vision.arucoc_finder import ArucoFinder
from gobot_vision.cell_scanner import CellScanner
import cv2, numpy


class GobotVision():

    def __init__(self):
        '''
        1. Overview of origin_image.
        2. Process:
           * Find all aruco marks, and there position
           * Get perspectived veiws of command_image, board_image, house_vendor_image
           * Scan the segmented images, To get command, board_layout, house_vender_stone.
        '''

        self.aruco_finder = ArucoFinder([21,49,48,15,13,34])
        '''
        TODO:  update 15 to 10
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

        logging.warn('Init vision is done......')

    def init_chessboard_layout(self):
        self.__chessboard_scanner.create_blank_layout()
        
    def ProcessOriginImage(self, origin_image, print_report:bool) ->bool:
        '''
        * Return false, If could not detect all known aruco marks. 
        ### After this processing,  Below properties will be set.
        * self.all_marks
        * self.perspectived_image
        * self.house_vender_image (is perspectived, and cropped)
        * self.board_image(is perspectived, and cropped)
        '''
        self.all_marks = self.aruco_finder.ScanMarks(origin_image=origin_image,print_report=print_report)
        if self.all_marks is None:
            print('[Warn] GobotVision  ProcessOriginImage(), ScanMarks() returns bad')
            return False
        mark_points = self.aruco_finder.GetPoints_For_PespectiveInput()
        if mark_points is None:
            print('[Warn] GobotVision  ProcessOriginImage(), GetPoints_For_PespectiveInput() returns bad')
            return False
        transformer = PerspectiveTransformer()
        self.perspectived_image = transformer.get_perspective_view(origin_image, mark_points)
        ImageLogger.Output("perspectived_image", self.perspectived_image, to_where=ImageLoggerToWhere.TO_SCREEN)
        return True

    def GetChessboardLayout(self):
        # is_ok = self.ProcessOriginImage(origin_image)
        # if not is_ok:
        #     return None, -1
        width = 450
        height = 428
        y1= 0
        y2= y1 + height
        x1= 0
        x2= x1 + width
        board_image = self.perspectived_image[y1:y2, x1:x2]
        # rotated_board_image = Img_RotateScale(board_image, angle_in_degree=180)
        finnal_board_image = cv2.flip(board_image, flipCode=0)
        ImageLogger.Output("final_board_image", finnal_board_image)
        layout, stable_depth = self.__chessboard_scanner.StartScan(finnal_board_image, history_length=3, show_processing_image=True)
        return layout, stable_depth
        
    def GetHouseVenderStone(self):
        y1= 550
        y2= y1 + 30
        x1= 200
        x2= x1 + 30
        house_vender_image = self.perspectived_image[y1:y2, x1:x2]
        board_gray = cv2.cvtColor(self.board_image, cv2.COLOR_BGR2GRAY)
        board_brightness = numpy.mean(board_gray)
        cell_scanner = CellScanner(board_brightness)
        house_vendor_stone_color = cell_scanner.ScanWhite(house_vender_image, is_inspected=False)
        return house_vendor_stone_color 

    def get_stable_level (self, layout_history):
        stable_level = 0
        if layout_history[0][0][0] == layout_history[1][0][0]:
            stable_level += 1
        return stable_level

    def get_command_index(self, origin_image):
        return self.__commander.get_command_from_image(origin_image)


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
    
