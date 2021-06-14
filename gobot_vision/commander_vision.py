class config_2_aruco_marks:
    class right:
        id = 49
    class left:
        id = 48


class CommanderVision():
    def __init__(self):
        self.layout = self.create_blank_layout()

    def create_2_aruco_marks_config(self):
        return config_2_aruco_marks

    def create_blank_layout(self):
        layout = [(0) * 5]
        return layout

    def scan_command(self, origin_image):
        return -1