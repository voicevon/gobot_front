
class config_2_aruco_marks:
    class right:
        id = 49
    class left:
        id = 48

class config_4_aruco_marks:
    class top_right:
        id = 1
    class bottom_right:
        id = 2
    class bottom_left:
        id = 3
    class top_left:
        id = 4

class RobotVision():
    def __init__(self):
        pass

    def get_2_aruco_marks_config(self):
        return config_2_aruco_marks

    def get_4_aruco_marks_config(self):
        return config_4_aruco_marks

    def detect_grid_from_aruco_corners(self, config, image):
        return image


BLANK = 0
WHITE_STONE = 1
BLACK_STONE = 2

class Stone():
    def __init__(self):
        self.type = BLANK

class GobotVision(RobotVision):
    def __init__(self):
        '''
        '''
        RobotVision.__init__(self)

    def get_commander_plate(self):
        return 1

    def get_chessboard_plate(self):
        return 1

    def get_chessboard_layout(self, image):
        layout = [Stone]
        return layout
    
    def get_warehouse_plate(self):
        return 1

    def get_warehouse_stone_position(self):
        return 1
