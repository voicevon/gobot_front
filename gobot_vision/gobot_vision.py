from gobot_vision.chessboard_vision import ChessboardVision
from gobot_vision.warehouse_vision import WarehouseVision




class RobotVision():
    def __init__(self):
        pass

    def detect_grid_from_aruco_corners(self, config, image):
        return image




class GobotVision(RobotVision):
    def __init__(self):
        '''
        '''
        RobotVision.__init__(self)
        self.__chessboard_vision = ChessboardVision()

    def get_commander_plate(self):
        return 1


    def get_stable_level (self, layout_history):
        stable_level = 0
        if layout_history[0][0][0] == layout_history[1][0][0]:
            stable_level += 1
        return stable_level

    def get_chessboard_plate(self, origin_image):
        config = self.__chessboard_vision.get_4_aruco_marks_config()
        board_image = self.detect_grid_from_aruco_corners(config, origin_image)
        return board_image

    def get_chessboard_layout(self, image, stable_level=3, max_trying = 6):
        history = []
        layout = self.__chessboard_vision.get_layout_from_image(image)
        history.append(layout)
        stable_count = self.get_stable_level(history)
        if stable_count >= stable_level:
            return True,layout
        
            

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
    
