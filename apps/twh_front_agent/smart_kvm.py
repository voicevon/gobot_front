import cv2


class SmartKvmTargetArea:
    name = "new_target_area"
    mark_filename = ""
    top_offset = 0
    left_offset = 0
    width = 100
    height = 100



class SmartKvmConfig:
    target_areas = []


class SmartKvm:
    def __init__(self, camera_id, config) -> None:
        self.__cap = cv2.VideoCapture(camera_id)
        self.__config = config
