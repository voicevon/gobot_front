
from enum import Enum
from tkinter import Image
import cv2

class ImageLoggerToWhere(Enum):
    TO_MQTT = 1
    TO_SCREEN = 2
    TO_FILE = 3

class ImageLogger():
    @property
    def to_where() :ImageLoggerToWhere

    @staticmethod
    def SetOutputToWhere(to_somewhere : ImageLoggerToWhere):
        ImageLogger.to_where = to_somewhere

    # @property
    # def mute_image_list(): ['eye/origin', 'command','grid/aruco','gobot/image/board']

    @staticmethod
    def Output(topic_or_title: str, cv_image):
        # if topic_or_title in ImageLogger.mute_image_list:
        mute_topic = ['eye/origin', 'command','grid/aruco','gobot/image/board']
        if topic_or_title in mute_topic:
            return

        if ImageLogger.to_where == ImageLoggerToWhere.TO_SCREEN:
            cv2.imshow(topic_or_title, cv_image)
            cv2.waitKey(1)
        else:
            print("ImageLogger:Output", "to_where is not understandable.....", ImageLogger.to_where)


    # print('Prepare MQTT connection......')
    # Config.publish_mqtt = True
    # if Config.publish_mqtt:
    #     g_mqtt.append_configable_var(Config)
    #     g_mqtt.connect_to_broker('123457','voicevon.vicp.io',1883,'von','von1970')