
from enum import Enum
import cv2
import logging

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

    @staticmethod
    def Output(topic_or_title: str, cv_image):
        if ImageLogger.to_where == ImageLoggerToWhere.TO_SCREEN:
            cv2.imshow(topic_or_title, cv_image)
        else:
            logging.error("to_where is not understandable.....")


    # print('Prepare MQTT connection......')
    # Config.publish_mqtt = True
    # if Config.publish_mqtt:
    #     g_mqtt.append_configable_var(Config)
    #     g_mqtt.connect_to_broker('123457','voicevon.vicp.io',1883,'von','von1970')