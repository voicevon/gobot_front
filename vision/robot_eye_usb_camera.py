
import sys
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 
from vision.robot_eye_base import MonoEyeBase
import cv2

class MonoEyeUsbCamera(MonoEyeBase):
    def __init__(self, coefficients_file):
        self.__camera = cv2.VideoCapture(0, cv2.CAP_DSHOW)

    def take_picture(self, do_undistort=True):
        return_value, image = self.__camera.read()
        if do_undistort:
            if super.__show_debug_info:
                print('RobotEye.take_picture()   start undistortion')
            undistort_image = cv2.undistort(image, self.__mtx, self.__dist, None, None)
            if super.__show_debug_info:
                print('...... end undistortion')
            return undistort_image
        return image

    def LinkCamera():
        pass

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