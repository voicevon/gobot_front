from von.logger import Logger
from von.mqtt.mqtt_agent import g_mqtt
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
import cv2
import time


class KvmNodeCamera:
    def __init__(self, os:str, config_getter: RemoteVar_mqtt) -> None:
        '''
        os = {'Windows', 'Linux_desktop','Pi_lite'}
        '''
        # self.__config_getter = config_getter
        self.__config, _ = config_getter.get_json()
        self.node_name = self.__config['node_name']
        self.mqtt_topic_of_screen_image = 'ocr/kvm/' + self.node_name + '/screen_image'
        self.__OS = os
        # self.fps = self.__config['fps']
        
        if os == 'Windows':
            self.__cv2_capture = cv2.VideoCapture(0, cv2.CAP_DSHOW)
        else:
            self.__cv2_capture = cv2.VideoCapture(0, cv2.CAP_V4L2)

        width, height =  self.__config['resolution']  # (1280,720)
        self.__cv2_capture.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        self.__cv2_capture.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
        self.__start_time = 0

    def Capture_Image(self):
        ret, frame = self.__cv2_capture.read()
        if ret:
            return frame
        else:
            Logger.Error("KvmNode::SpinOnce()  Carmra did NOT return frame.")      
            return None

    
    def publish(self, image):
        '''
        Follow the confg['fps']
        '''
        now_time = time.time()
        if int(now_time - self.__start_time) > self.__config['fps']:
            # if self.__OS !='Pi_lite':
            #     cv2.imshow('camera', image)
            #     cv2.waitKey(1)
            bytes_count =  g_mqtt.publish_cv_image(self.mqtt_topic_of_screen_image,  image)
            self.__start_time = time.time()
            if self.__OS != "Pi_lite_no_user":
                Logger.Print(self.node_name +  " published to: " + self.mqtt_topic_of_screen_image  + "  bytes (KB)", bytes_count / 1000)
        