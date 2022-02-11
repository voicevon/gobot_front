
import sys
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 

from numpy import true_divide
from vision.robot_eye_base import MonoEyeBase
import cv2
import logging

class MonoEyeUsbCamera(MonoEyeBase):

    # def show_debug_info(self) -> bool:
        # return super().show_debug_info

    def __init__(self, coefficients_file):
        self.__camera = cv2.VideoCapture(0, cv2.CAP_DSHOW)
        self.show_debug_info = True
        self.__coefficients_file = coefficients_file
        mtx, dist = self.load_coefficients(coefficients_file)
        self.__mtx = mtx
        self.__dist = dist
        self.__CALIBRATION_IMAGE_PATH = './camera_calibration_images/'
        self.__show_debug_info = False
        logging.warning('Init eye is done......')


    def take_picture(self, do_undistort=True):
        return_value, image = self.__camera.read()
        if do_undistort:
            if self.show_debug_info:
                print('RobotEye.take_picture()   start undistortion')
            undistort_image = cv2.undistort(image, self.__mtx, self.__dist, None, None)
            if self.show_debug_info:
                print('...... end undistortion')
            return undistort_image
        return image



    def take_batch_picture_for_calibration(self):
        pass
        
        
if __name__ == '__main__':

    my_eye = MonoEyeUsbCamera('2021-0611.yml')
    while False:
        # Video test
        image = my_eye.take_picture(do_undistort=False)
        cv2.imshow("Captured", image)
        cv2.waitKey(1)

    while True:
        # Video test. When Ctrl+C, there is an error
        image = my_eye.take_picture(do_undistort=False)
        cv2.imshow("Captured", image)
        cv2.waitKey(5000)

    if False:
        my_eye.take_batch_picture_for_calibration()
        my_eye.recalibrate_and_save_coefficients()