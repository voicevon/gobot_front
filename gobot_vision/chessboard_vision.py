

BLANK = 0
WHITE_STONE = 1
BLACK_STONE = 2


class config_4_aruco_marks:
    class top_right:
        id = 1
    class bottom_right:
        id = 2
    class bottom_left:
        id = 3
    class top_left:
        id = 4


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