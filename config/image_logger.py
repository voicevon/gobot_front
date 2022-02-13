
from enum import Enum
import logging
# from tkinter import Image
# from types import ClassMethodDescriptorType
# from tkinter import Image
import cv2
from von.mqtt_helper import g_mqtt

class ImageLoggerToWhere(Enum):
    TO_MQTT = 1
    TO_SCREEN = 2
    TO_FILE = 3

class ImageLogger():
    __to_where: ImageLoggerToWhere

    @property
    @classmethod
    def to_where(cls) ->ImageLoggerToWhere:
        return cls.__to_where

    @classmethod
    @to_where.setter
    def to_where(cls, value: ImageLoggerToWhere):
        cls.__to_where = value
        # Below is nerver worked!  a python bug?
        print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
        if value == ImageLoggerToWhere.TO_MQTT:
            logging.info("Connecting to MQTT broker......")
            g_mqtt.connect_to_broker("T22-0213",'voicevon.vicp.io',1883,'von','von1970')
    
    @staticmethod
    def connect_to_mqtt_broker():
        g_mqtt.connect_to_broker("T22-0213",'voicevon.vicp.io',1883,'von','von1970')



    @staticmethod
    def SetOutputToWhere(to_somewhere : ImageLoggerToWhere):
        ImageLogger.to_where = to_somewhere

    # @property
    # def mute_image_list(): ['eye/origin', 'command','grid/aruco','gobot/image/board']

    @staticmethod
    def Output(topic_or_title: str, cv_image):
        # if topic_or_title in ImageLogger.mute_image_list:
        # mute_topic = []
        mute_topic = ['xx eye/origin'
                        , 'command'
                        ,'grid/aruco'
                        ,'gobot/image/board']
        if topic_or_title in mute_topic:
            return

        if ImageLogger.to_where == ImageLoggerToWhere.TO_SCREEN:
            cv2.imshow(topic_or_title, cv_image)
            cv2.waitKey(1)
        if ImageLogger.to_where == ImageLoggerToWhere.TO_MQTT:
            g_mqtt.publish_cv_image(topic=topic_or_title,cv_image=cv_image, retain=True )

        else:
            print("ImageLogger:Output", "to_where is not understandable.....", ImageLogger.to_where)


    # print('Prepare MQTT connection......')
    # Config.publish_mqtt = True
    # if Config.publish_mqtt:
    #     g_mqtt.append_configable_var(Config)
    #     g_mqtt.connect_to_broker('123457','voicevon.vicp.io',1883,'von','von1970')