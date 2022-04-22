

import sys
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 

import cv2
from vision.robot_eye_base import MonoEyeBase


class MonoEyeEmulator(MonoEyeBase):
    MIN_FILE_ID = 1
    MAX_FILE_ID = 1

    def __init__(self) -> None:
        self.file_id = MonoEyeEmulator.MIN_FILE_ID

    def take_picture(self, do_undistort=True):
        if self.file_id > MonoEyeEmulator.MAX_FILE_ID:
            self.file_id = MonoEyeEmulator.MIN_FILE_ID
        # On winodows 10
        filename = "d:\\XumingSource\\gobot_front\\vision\\emulator_pictures\\" + str(self.file_id) + ".jpg"
        # On Rasp Pi Zero
        filename = "./vision/emulator_pictures/" + str(self.file_id) + ".jpg" 
        self.file_id += 1

        image = cv2.imread(filename)
        if image is None:
            print("[Error] robot_eye_emulator.py   take_picture() read from file, ", filename)
        # print("file_id= ", self.file_id)
        # cv2.imshow("ttttttttttt", image)
        dimention = (1920,1088)
        resized = cv2.resize(image, dimention, interpolation = cv2.INTER_AREA)
        return resized

if __name__ == '__main__':
        runner = MonoEyeEmulator()
        while True:
            image = runner.take_picture()
            cv2.imshow("test", image)
            cv2.waitKey(3000)