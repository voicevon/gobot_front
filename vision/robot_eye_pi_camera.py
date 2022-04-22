from picamera import PiCamera
from picamera.array import PiRGBArray  # sudo apt-get install python3-picamera

import cv2
import logging

import sys
sys.path.append('/home/pi/gobot_front')  # For runing in VsCode on Raspberry Pi
from vision.robot_eye_base import MonoEyeBase
from config.message_logger import MessageLoggerToWhere,MessageLogger
from config.image_logger import ImageLogger, ImageLoggerToWhere


class MonoEyePiCamera(MonoEyeBase):

    def __init__(self, coefficients_file):
        self.__camera = PiCamera(resolution=(1920,1088))
        self.__coefficients_file = coefficients_file
        mtx, dist = self.load_coefficients(coefficients_file)
        self.__mtx = mtx
        self.__dist = dist
        self.__CALIBRATION_IMAGE_PATH = './camera_calibration_images/'
        self.__show_debug_info = False
        logging.warning('MonoEyePiCamera.__Init() is done......')

    def take_picture(self, do_undistort=True):
        # print("[Info] robot_eye.pi_camera.py  take_picture()", "entering")
        rawCapture = PiRGBArray(self.__camera)
        # grab an image from the camera
        self.__camera.capture(rawCapture, format="bgr")
        image = rawCapture.array
        if do_undistort:
            if self.__show_debug_info:
                print('RobotEye.take_picture()   start undistortion')
            undistort_image = cv2.undistort(image, self.__mtx, self.__dist, None, None)
            if self.__show_debug_info:
                print('...... end undistortion')
            return undistort_image
        return image

        
if __name__ == '__main__':
    import sys
    from von.mqtt_helper import g_mqtt, MQTT_ConnectionConfig
    
    config = MQTT_ConnectionConfig()
    config.broker="voicevon.vicp.io"
    config.port = 1883
    config.client_id = "Y22-0422"
    config.uid = 'von'
    config.password = 'von1970'
    g_mqtt.connect_to_broker(config)

    my_eye = MonoEyePiCamera('2021-0611.yml')
    test_id = 1
    if test_id == 1:
        # test camera hardware, picture will be saved to file
        img = my_eye.take_picture(False)
        cv2.imwrite("test_camera.jpg", img)
        ImageLogger.to_where = ImageLoggerToWhere.TO_MQTT
        ImageLogger.Output("gobot/head/eye/origin", img)
        cv2.waitKey(5000)
        
    if False:
        # my_eye.take_batch_picture_for_calibration()
        my_eye.take_picture(do_undistort=true)
    if test_id==3:
        my_eye.recalibrate_and_save_coefficients()