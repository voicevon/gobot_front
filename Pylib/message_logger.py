
from enum import Enum
import logging

class MessageLoggerToWhere(Enum):
    TO_NONE = 0
    TO_MQTT = 1
    TO_SCREEN = 2
    TO_FILE = 3

class MessageLogger():
    @property
    def to_where() :MessageLoggerToWhere

    @staticmethod
    def SetOutputToWhere(to_somewhere : MessageLoggerToWhere):
        MessageLogger.to_where = to_somewhere

    @staticmethod
    def Output(topic_or_title: str, message):
        return
        mute_list = ['GridFinder.find_corners()  double check target ids',
                    'GridFinder.find_corners()  found ids',
                    'CellScanner.scan_black()  scan_black_counter =',
                    'CellScanner.scan_white()',
                    'scan black bin',
                    '>>>>average_brightness=',
                    'user_play_Stable_depth',
                    ]
        if topic_or_title in mute_list: 
            return

        if MessageLogger.to_where == MessageLoggerToWhere.TO_SCREEN:
            print(topic_or_title, message)
        else:
            print("MessageLogger:Output", "to_where is not understandable.....", MessageLogger.to_where)


    # print('Prepare MQTT connection......')
    # Config.publish_mqtt = True
    # if Config.publish_mqtt:
    #     g_mqtt.append_configable_var(Config)
    #     g_mqtt.connect_to_broker('123457','voicevon.vicp.io',1883,'von','von1970')