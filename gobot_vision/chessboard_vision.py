

BLANK = 0
WHITE_STONE = 1
BLACK_STONE = 2


class config_4_aruco_marks:
    name='CHESSBOARD_ARUCO_CONFIG'
    top_right_id =  1
    bottom_right_id = 2
    bottom_left_id = 3
    top_left_id = 3
    width = 640
    height = 640
    


class ChessboardVision():
    def __init__(self):
        self.layout = self.create_blank_layout()

    def create_blank_layout(self):
        layout = [([0] * 19) for i in range(19)]
        return layout

    def get_layout_from_image(self, chessboard_image):
        return self.layout

    def get_4_aruco_marks_config(self):
        return config_4_aruco_marks
