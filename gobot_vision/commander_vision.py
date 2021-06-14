class config_2_aruco_marks:
    class right:
        id = 49
    class left:
        id = 48


class CommanderVision():
    def __init__(self):
        self.layout = [(0) * 5]

    def get_2_aruco_marks_config(self):
        return config_2_aruco_marks
