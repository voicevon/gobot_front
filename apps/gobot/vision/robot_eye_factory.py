from vision.robot_eye_base import MonoEyeBase


import sys
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 



import cv2
from enum import Enum

class RobotEye_Product(Enum):
    PaspberryPiCamera = 1
    UsbCamera = 2
    CameraEmulator = 3

class RobotEye_Factory():

    @staticmethod
    def CreatePiCameraEye() -> MonoEyeBase:
        from vision.robot_eye_pi_camera import MonoEyePiCamera
        return MonoEyePiCamera('2021-0611.yml')

    @staticmethod
    def CreateUsbCameraEye() -> MonoEyeBase:
        from vision.robot_eye_usb_camera import MonoEyeUsbCamera
        return MonoEyeUsbCamera('2021-0611.yml')

    @staticmethod
    def CreateCameraEmulator() -> MonoEyeBase:
        from vision.robot_eye_emulator import MonoEyeEmulator
        obj = MonoEyeEmulator()
        return obj
        
    @staticmethod
    def CreateMonoEye(eye: RobotEye_Product) -> MonoEyeBase:
        if eye == RobotEye_Product.UsbCamera:
            return RobotEye_Factory.CreateUsbCameraEye()

        if eye == RobotEye_Product.CameraEmulator:
            return RobotEye_Factory.CreateCameraEmulator()

        if eye == RobotEye_Product.PaspberryPiCamera:
            return RobotEye_Factory.CreatePiCameraEye()


        
if __name__ == '__main__':

    my_eye = RobotEye_Factory.CreateMonoEye(RobotEye_Product.UsbCamera)
    image = my_eye.take_picture(do_undistort=False)
    cv2.imshow("Captured", image)
    cv2.waitKey(5000)
    if False:
        my_eye.take_batch_picture_for_calibration()
        my_eye.recalibrate_and_save_coefficients()

