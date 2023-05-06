import cv2
from von.mqtt_agent import g_mqtt, g_mqtt_broker_config
from von.logger import Logger
from von.remote_var_mqtt import RemoteVar_mqtt
import time



class KvmNode:
    def __init__(self, os:str, json_config) -> None:
        '''
        os = {'Windows', 'Linux_desktop','Pi_lite'}
        '''
        self.node_name = json_config['node_name']
        self.__mqtt_topic_of_screen_image = 'ocr/' + self.node_name + '/screen_image'
        self.fps = json_config['fps']
        self.__OS = os
        if self.__OS == 'Windows':
            self.__cv2_capture = cv2.VideoCapture(0, cv2.CAP_DSHOW)
        else:
            self.__cv2_capture = cv2.VideoCapture(0, cv2.CAP_V4L2)

        self.__resolution =  json_config['resolution']  # (1280,720)
        width, height = self.__resolution
        self.__cv2_capture.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        self.__cv2_capture.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
        self.__start_time = 0

    def SpinOnce(self):
        ret, frame = self.__cv2_capture.read()
        if not ret:
            Logger.Error("KvmNode::SpinOnce()  Carmra did NOT return frame.")

        now_time = time.time()
        if int(now_time - self.__start_time) > self.fps:
            if self.__OS !='Pi_lite':
                cv2.imshow('camera', frame)
                cv2.waitKey(1)
            bytes_count =  g_mqtt.publish_cv_image(self.__mqtt_topic_of_screen_image,  frame)
            self.__start_time = time.time()
            Logger.Print("published, image in bytes count", bytes_count / 1000)
        

node_name = 'ubuntu_230506'

if __name__ == '__main__':
    g_mqtt_broker_config.client_id = node_name
    g_mqtt.connect_to_broker(g_mqtt_broker_config)

    mqtt_topic_of_node_config = 'ocr/' + node_name + '/config'
    config_json = RemoteVar_mqtt(mqtt_topic_of_node_config, None)
    while not config_json.rx_buffer_has_been_updated():
        pass
    kvm_node = KvmNode('Windows',config_json)
    # kvm_node = KvmNode('Pi_lite',config_json)


    while True:
        kvm_node.SpinOnce()
        # ret, frame = cap.read()
        # if ret:
        #     now_time = time.time()
        #     if int(now_time - start_time) > int(fps_limit.get()):
        #         if os !='Pi_lite':
        #             cv2.imshow('camera', frame)
        #             cv2.waitKey(1)
        #         bytes_count =  g_mqtt.publish_cv_image("ocr/ubuntu_performance/screen_image",  frame)
        #         start_time = time.time()

        #         publish_counter += 1
        #         Logger.Print("published, image in bytes count", bytes_count / 1000)


