
from enum import Enum
import logging
import cv2
from von.mqtt_helper import g_mqtt, MQTT_ConnectionConfig
from Pylib.rabbit_mq_helper import g_amq

class ImageLoggerToWhere(Enum):
    TO_MQTT = 1
    TO_AMQ = 2
    TO_SCREEN = 3
    TO_FILE = 4
    FOLLOW_SETTING = 99

class ImageLogger():
    __to_where: ImageLoggerToWhere

    @property
    @classmethod
    def to_where(cls) ->ImageLoggerToWhere:
        return cls.__to_where

    @property
    @to_where.setter
    def to_where(cls, value: ImageLoggerToWhere):
        cls.__to_where = value
        # Below is nerver worked!  a python bug?
        print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
        if value == ImageLoggerToWhere.TO_MQTT:
            logging.info("Connecting to MQTT broker......")
            config = MQTT_ConnectionConfig()
            config.broker="voicevon.vicp.io"
            config.port = 1883
            config.client_id = "Y22-0422"
            config.uid = 'von'
            config.password = 'von1970'
            g_mqtt.connect_to_broker(config)
    
    @staticmethod
    def connect_to_mqtt_broker():
        config = MQTT_ConnectionConfig()
        config.broker="voicevon.vicp.io"
        config.port = 1883
        config.client_id = "Y22-0422"
        config.uid = 'von'
        config.password = 'von1970'
        g_mqtt.connect_to_broker(config)



    @staticmethod
    def SetOutputToWhere(to_somewhere : ImageLoggerToWhere):
        ImageLogger.to_where = to_somewhere

    # @property
    # def mute_image_list(): ['eye/origin', 'command','grid/aruco','gobot/image/board']

    @staticmethod
    def Output(topic_or_title: str, cv_image, to_where = ImageLoggerToWhere.FOLLOW_SETTING):
        # if topic_or_title in ImageLogger.mute_image_list:
        # mute_topic = []
        mute_topic = ['ggg_gobot/head/eye/origin'
                        ,'command'
                        ,'grid/aruco'
                        ,'gobot/image/board'
                        ]
        if topic_or_title in mute_topic:
            return
        if to_where == ImageLoggerToWhere.FOLLOW_SETTING:
            to_where = ImageLogger.to_where
            
        if to_where == ImageLoggerToWhere.TO_SCREEN:
            try:
                cv2.imshow(topic_or_title, cv_image)
                cv2.waitKey(1)
            except:
                pass

        elif to_where == ImageLoggerToWhere.TO_MQTT:
            print("[Warn] ImageLogger::Output() TO_MQTT is out of service.")
            return
            g_mqtt.publish_cv_image(topic=topic_or_title,cv_image=cv_image, retain=True )
        elif to_where == ImageLoggerToWhere.TO_AMQ:
            g_amq.publish_cv_image("gobot_x2134_eye_origin", cv_image)
        
        else:
            print("[Error] ImageLogger::Output()", "to_where is not understandable.....", ImageLogger.to_where)


    # print('Prepare MQTT connection......')
    # Config.publish_mqtt = True
    # if Config.publish_mqtt:
    #     g_mqtt.append_configable_var(Config)
    #     g_mqtt.connect_to_broker('123457','voicevon.vicp.io',1883,'von','von1970')