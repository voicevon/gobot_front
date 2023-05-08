from von.mqtt.remote_var_mqtt import RemoteVar_mqtt, g_mqtt
from von.logger import Logger
import json
import cv2
import numpy
import pytesseract
from PIL import Image #pip install pillow


class ScreenImageDivider:

    def __init__(self, app_window_name:str, auto_init_config=False) -> None:
        '''
        '''
        self.__config = {}
        self.__config_getter = RemoteVar_mqtt("ocr/" + app_window_name + "/config", None, True)
        self.__mqtt_topic_of_marker_image = "ocr/" + app_window_name + "/marker_image"
        if auto_init_config:
            # create default config, and save it to mqtt
            self.__config['areas'] = {}
            self.__marker_image = numpy.zeros((100, 100, 3), numpy.uint8)
            self.__save_config()
            g_mqtt.publish_cv_image(self.__mqtt_topic_of_marker_image, self.__marker_image)
            
        else:
            self.__config, is_new = self.__config_getter.get_json()
            # marker image
            marker_getter = RemoteVar_mqtt(self.__mqtt_topic_of_marker_image, None, True)
            self.__marker_image = marker_getter.get_cv_image()
         # width, height = 1024, 768
        # self.__frame = numpy.zeros((width, height,3), numpy.uint8)

    def update_areas(self, areas_json):
        '''
        areas will be updated to config,  and config will be saved to mqtt-broker
        '''
        self.__config["areas"] = areas_json
        self.__save_config()

    def __save_config(self):
        payload = json.dumps(self.__config)
        # Logger.Print('ssssssssssssssss', payload)
        self.__config_getter.set(payload)
        # marker_getter = RemoteVar_mqtt(self.__config['mqtt_topic_of_marker_image'], None)


        
    def show_config(self):
        pass
        # print(self.__config["areas"][1]['postions'])
        # for i in range(1,2):
        #     x1,y1,x2,y2 = self.__config["areas"][i]['postions']
        #     cv2.rectangle(self.__frame, (x1,y1),(x2,y2), (0,255,0), thickness=2)
        # cv2.imshow("debug", self.__frame)


            
    def DivideToSmallImages(self, window_image):
        '''
        return a list of small images
        '''
        # if not self.__image_getter.rx_buffer_has_been_updated():
        #     return

        # np_array = numpy.frombuffer(self.__image_getter.get(), dtype=numpy.uint8) 
        # self.__frame = cv2.imdecode(np_array, flags=1)

        # Now we got a frame, try to  find marker.  
        # left, top = self.match_template(self.__frame)
        left, top = 0, 0

        # crop frame to many ocr units, base location is the marker.
        # Logger.Print("ppppppppppppppp", self.__config["areas"]["areas"])
        # x2,y2, x1,y1 = self.__config["areas"][1]['postions']
        x1,y1, x2,y2 = self.__config["areas"][1]['postions']

        # print(x1,y1, x2,y2)
        cropped_image = window_image[y1:y2, x1:x2]
        cv2.imshow("area-1", cropped_image)
        cv2.waitKey(1)
        color_converted = cv2.cvtColor(cropped_image, cv2.COLOR_BGR2RGB)
        pil_image =  Image.fromarray(color_converted)
        string = pytesseract.image_to_string(pil_image)
        Logger.Print("pytesseract string", string)


        # for unit in self.__all_units:
        #     image = unit.GetAreaImage(self.__frame, left, top)
        #     cv2.imshow(unit.name, image)
        #     cv2.waitKey(1)



