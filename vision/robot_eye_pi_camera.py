from picamera import PiCamera
from picamera.array import PiRGBArray  # sudo apt-get install python3-picamera
from robot_eye import MonoEyeBase

class MonoEyePiCamera(MonoEyeBase):
    def take_picture(self, do_undistort=True):
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

    def LinkCamera():
        super.__camera = PiCamera(resolution=(1920,1088))
        