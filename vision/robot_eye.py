
from math import sin, cos
from picamera.array import PiRGBArray
from picamera import PiCamera



class MonoEye():
    '''
    take picture from Pi camera, 
    TODO: Calibration
    '''

    def __init__(self):
        self.__camera = PiCamera(resolution=(1920,1088))
        #self.__rawCapture = PiRGBArray(self.__camera)
    def take_picture(self):
        rawCapture = PiRGBArray(self.__camera)
        # grab an image from the camera
        self.__camera.capture(rawCapture, format="bgr")
        image = rawCapture.array
        return image

class SteroEye():
    pass
