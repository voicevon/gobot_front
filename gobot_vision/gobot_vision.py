from gobot_vision.commander_vision import CommanderVision
from vision.grid_finder import GridFinder
from gobot_vision.chessboard_vision import ChessboardVision, config_4_aruco_marks
from gobot_vision.warehouse_vision import WarehouseVision
import cv2
import numpy as np



class RobotVision():
    def __init__(self):
        self.__grid_finder = GridFinder()

    def get_perspective_view(self, img, pts):
        # specify desired output size 
        width = self.__area_width
        height = self.__area_height

        # specify conjugate x,y coordinates (not y,x)
        input = np.float32(pts)
        output = np.float32([[0,0], [width-1,0], [width-1,height-1], [0,height-1]])

        # compute perspective matrix
        matrix = cv2.getPerspectiveTransform(input,output)

        # print(matrix.shape)
        # print(matrix)

        # do perspective transformation setting area outside input to black
        imgOutput = cv2.warpPerspective(img, matrix, (width,height), cv2.INTER_LINEAR, borderMode=cv2.BORDER_CONSTANT, borderValue=(0,0,0))
        # print(imgOutput.shape)

        # save the warped output
        return imgOutput

    def detect_grid_from_aruco_corners(self, config, origin_image):
        grid_finder = GridFinder(config)
        # Get corners position from detecting aruco marks
        corners = grid_finder.find_corners(origin_image)
        # print(corners)
        if corners != None:
            if len(corners) >= len(self.__mark_ids):
                # Get perspectived image
                perspect_img = self.get_perspective_view(origin_image,corners)
                return perspect_img
        return None




class GobotVision(RobotVision):
    def __init__(self):
        '''
        '''
        RobotVision.__init__(self)
        self.__chessboard_vision = ChessboardVision()
        self.__commander_vision = CommanderVision()


    def get_stable_level (self, layout_history):
        stable_level = 0
        if layout_history[0][0][0] == layout_history[1][0][0]:
            stable_level += 1
        return stable_level
        
    

    def get_commander_plate(self, origin_image):
        config = self.__commander_vision.create_2_aruco_marks_config()
        commander_image = self.detect_grid_from_aruco_corners(config, origin_image)
        return commander_image

    def get_commander_layout(self, image, min_stable_depth=3, max_trying =5):
        history = []
        layout = self.__chessboard_vision.get_layout_from_image(image)
        history.append(layout)
        stable_count = self.get_stable_level(history)
        if stable_count >= min_stable_depth:
            return True,layout
        
        return False, -1



    def get_chessboard_plate(self, origin_image):
        config = self.__chessboard_vision.get_4_aruco_marks_config()
        board_image = self.detect_grid_from_aruco_corners(config, origin_image)
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
    
