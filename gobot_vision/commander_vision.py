class config_2_aruco_marks:
    name = 'COMMANDER_ARUCO_CONFIG'
    right_id = 49
    left_id = 48


class CommanderVision():
    def __init__(self):
        '''
        For the reason: deep decoupling, 
            We will never put origin_image or GridFinder into this class.
        '''
        self.layout = self.create_blank_layout()

    def create_2_aruco_marks_config(self):
        return config_2_aruco_marks

    def create_blank_layout(self):
        layout = [(0) * 5]
        return layout

    def convert_to_layout(self, origin_image):
        layout = self.get_commander_layout(origin_image, min_stable_depth=3, max_trying=5)
        for i in range(0, layout.count,1):
            if layout[i]:
                command = i
                break
