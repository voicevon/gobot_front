
import sys
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 

import cv2


class RobotEyeFactory():

    @staticmethod
    def CreatePiCameraEye():
        from robot_eye_pi_camera import MonoEyePiCamera
        return MonoEyePiCamera('2021-0611.yml')

    @staticmethod
    def CreateUsbCameraEye():
        from vision.robot_eye_usb_camera import MonoEyeUsbCamera
        return MonoEyeUsbCamera('2021-0611.yml')


        
if __name__ == '__main__':

    my_eye = RobotEyeFactory.CreateUsbCameraEye()
    image = my_eye.take_picture(do_undistort=False)
    cv2.imshow("Captured", image)
    cv2.waitKey(5000)
    if False:
        my_eye.take_batch_picture_for_calibration()
        my_eye.recalibrate_and_save_coefficients()

