from ocr_unit import OcrUnit
from ocr_window import OcrWindow

from von.logger import Logger
from von.remote_var_mqtt import RemoteVar_mqtt

import cv2



class OcrFactory:
    
    @classmethod
    def CreateOcrWindow(cls, window_name:str) -> OcrWindow:
        '''
        Create ocr_window and config it.
        Load config from mqtt
        '''
        if window_name == 'ubuntu_performance':
            mqtt_topic_of_position_config = "ocr/" + window_name + "/config"
            mqtt_topic_of_screen_image = "ocr/" + window_name + "/screen_image"
            template_path_filename = "template_images/" + window_name + ".png"

            positions_config =  RemoteVar_mqtt(mqtt_topic_of_position_config, None)
            while not positions_config.rx_buffer_has_been_renewed:
                # wait mqtt syncing in the other thread.
                pass
            window_config = positions_config.get()
            window_config["template_image"] = cv2.imread(template_path_filename)
            new_windows = OcrWindow(window_config, 
                                    mqtt_topic_of_image = mqtt_topic_of_screen_image)
            return new_windows
        
        else:
            Logger.Error("OcrFactory::CreateOcrWindow()")
            Logger.Print('requested window_name', window_name)
            return None # type: ignore

    

    @classmethod
    def CreateOcrUnit(cls, unit_name:str) -> OcrUnit:
        if unit_name == 'title':
            unit = OcrUnit()
            unit.name = unit_name
            unit.height = 660
            unit.width = 450
            unit.left_offset = -20
            unit.top_offset = 50
            return unit
        else:
            Logger.Error('CreateOcrUnit()')
            Logger.Print('request unit_name', unit_name)
            return None # type: ignore