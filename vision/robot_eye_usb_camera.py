from robot_eye import MonoEyeBase
import cv2

class MonoEyeUsbCamera(MonoEyeBase):
    def __init__(self, coefficients_file):
        super().__init__(coefficients_file)

    def take_picture(self, do_undistort=True):
        # grab an image from the camera
        # self.__camera.capture(rawCapture, format="bgr")
        image = None
        if do_undistort:
            if self.__show_debug_info:
                print('RobotEye.take_picture()   start undistortion')
            undistort_image = cv2.undistort(image, self.__mtx, self.__dist, None, None)
            if self.__show_debug_info:
                print('...... end undistortion')
            return undistort_image
        return image

    def LinkCamera():
        pass
        
        
if __name__ == '__main__':

    my_eye = MonoEyeUsbCamera('2021-0611.yml')
    my_eye.take_picture(do_undistort=False)
    if False:
        my_eye.take_batch_picture_for_calibration()
    my_eye.recalibrate_and_save_coefficients()