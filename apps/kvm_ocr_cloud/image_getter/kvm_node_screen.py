from von.logger import Logger
from von.mqtt.mqtt_agent import g_mqtt
import cv2
import numpy as np
from mss import mss      # pip install mss
from PIL import Image
import time

# https://stackoverflow.com/questions/35097837/capture-video-data-from-screen-in-python

class KvmNodeScreen:
    def __init__(self, os:str, json_config) -> None:
        '''
        os = {'Windows', 'Linux_desktop','Pi_lite'}
        '''
        self.node_name = json_config['node_name']
        self.__mqtt_topic_of_screen_image = 'ocr/kvm/' + self.node_name + '/screen_image'
        self.__fps = json_config['fps']
        self.__resolution = json_config['resolution']
        self.__OS = os
        # if self.__OS == 'Windows':
        #     self.__cv2_capture = cv2.VideoCapture(0, cv2.CAP_DSHOW)
        # else:
        #     self.__cv2_capture = cv2.VideoCapture(0, cv2.CAP_V4L2)

        # self.__resolution =  json_config['resolution']  # (1280,720)
        # width, height = self.__resolution
        # self.__cv2_capture.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        # self.__cv2_capture.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
        self.__start_time = 0



    def Capture_Image(self):
        with mss() as sct:
            screenShot = sct.grab(self.__resolution)
            pil_img = Image.frombytes('RGB', (screenShot.width, screenShot.height), screenShot.rgb)
            # img = Image.frombytes('P', (screenShot.width, screenShot.height), screenShot.rgb)

            cv_img_bgr = np.array(pil_img)
            cv_img = cv2.cvtColor(cv_img_bgr, cv2.COLOR_BGR2RGB)
            return cv_img

    def publish(self, image):
        '''
        '''
        now_time = time.time()
        if int(now_time - self.__start_time) > self.__fps:
            if self.__OS !='Pi_lite':
                cv2.imshow('camera', image)
                cv2.waitKey(1)
            bytes_count =  g_mqtt.publish_cv_image(self.__mqtt_topic_of_screen_image,  image)
            self.__start_time = time.time()
            Logger.Print(self.node_name +  " published to: " + self.__mqtt_topic_of_screen_image  + "  bytes (KB)", bytes_count / 1000)
        