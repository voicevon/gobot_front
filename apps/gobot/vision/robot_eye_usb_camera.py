
import sys
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 
from vision.robot_eye_base import MonoEyeBase
import cv2
import logging

class MonoEyeUsbCamera(MonoEyeBase):

    # def show_debug_info(self) -> bool:
        # return super().show_debug_info

    def __init__(self, coefficients_file=None):
        self.__camera = cv2.VideoCapture(0, cv2.CAP_DSHOW)
        # self.__camera = cv2.VideoCapture(1, cv2.CAP_DSHOW)
        self.show_debug_info = True

        if coefficients_file is not None:
            self.__coefficients_file = coefficients_file
            mtx, dist = self.load_coefficients(coefficients_file)
            self.__mtx = mtx
            self.__dist = dist
        self.__CALIBRATION_IMAGE_PATH = './camera_calibration_images/'
        logging.info('Init eye is done......')


    def take_picture(self, do_undistort=False):
        return_value, image = self.__camera.read()
        if do_undistort:
            undistort_image = cv2.undistort(image, self.__mtx, self.__dist, None, None)
            return undistort_image
        return image



    # def take_batch_picture_for_calibration(self):
    #     pass
        
        
if __name__ == '__main__':

    test_code = 'Test capture'
    # test_code = 'take batch images for calibration'
    # test_code = 'calibrate camera'

    print('\n\n\n test_code == %s \n\n\n'%(test_code))

    my_eye = MonoEyeUsbCamera()
    while test_code == 'Test capture':
        # Video test
        image = my_eye.take_picture(do_undistort=False)
        cv2.imshow("Captured", image)
        cv2.waitKey(1)

    while False:
        # Video test. When Ctrl+C, there is an error
        image = my_eye.take_picture(do_undistort=False)
        cv2.imshow("Captured", image)
        cv2.waitKey(1)

    while test_code == 'take batch images for calibration':
        my_eye.take_batch_picture_for_calibration()

    while test_code == 'calibrate camera':
        my_eye.recalibrate_and_save_coefficients()