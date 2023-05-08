from von.logger import Logger
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt, g_mqtt
from libs.kvm_node_camera import KvmNodeCamera
from libs.kvm_node_screen import KvmNodeScreen
from libs.app_window_idenifier import AppWindowIdentifier
from libs.screen_image_divider import ScreenImageDivider
import pytesseract
import cv2


class OcrNode:
    def __init__(self, routing) -> None:
        self.routing = routing
        self.from_screen_capture = routing['from_screen_capture']
        self.from_camera_capture = routing['from_camera_capture']
        self.from_mqtt = routing['from_mqtt']
        self.screen_image_to_mqtt = routing['screen_image_to_mqtt']
        self.screen_image_to_app_window_idendifier = routing['screen_image_to_app_window_identifier']
        self.screen_image_to_image_divder = routing['screen_image_to_image_divider']
        self.screen_image_to_ocr = routing['screen_image_to_ocr']
        self.screen_image_to_areas_marker = routing['screen_image_to_areas_marker']
        self.small_images_to_ocr = routing['small_images_to_ocr']
        self.small_strings_to_mqtt = routing['small_strings_to_mqtt']
        self.__kvm_node_name = routing['kvm_node_name']
        self.__app_window_name = routing['app_window_name']
        self.__my_os = routing['my_os']

        if self.from_camera_capture:
            getter = RemoteVar_mqtt('ocr/kvm/' + self.__kvm_node_name + '/config', None, for_loading_config=True)
            self.__kvm_node = KvmNodeCamera(self.__my_os, getter)
        if self.from_screen_capture:
            config = {}
            config["node_name"] = self.__kvm_node_name
            config["fps"] = 1
            config['resolution'] = {'left': 0, 'top': 0, 'width': 1366, 'height': 768}
            self.__kvm_node = KvmNodeScreen(self.__my_os, config)

        if self.from_mqtt or self.screen_image_to_mqtt:
            mqtt_topic_of_screen_image = "ocr/kvm/" + self.__kvm_node_name + "/screen_image"
            self.__image_getter = RemoteVar_mqtt(mqtt_topic_of_screen_image, None)
        if self.screen_image_to_app_window_idendifier:
            self.__identifier = AppWindowIdentifier()
        if self.screen_image_to_image_divder: 
            getter =  RemoteVar_mqtt('ocr/' + self.__app_window_name + '/config', None, for_loading_config=True)
            self.__divider = ScreenImageDivider(self.__app_window_name)



    def SpinOnce(self):
        screen_image = None
        small_images = []
        small_strings=[]

        if self.from_camera_capture:
            screen_image = self.__kvm_node.Capture_Image()
        if self.from_screen_capture:
            screen_image = self.__kvm_node.Capture_Image()
        if self.from_mqtt:
            if self.__image_getter.rx_buffer_has_been_updated():
                screen_image, _ = self.__image_getter.get_cv_image()
                cv2.imshow("mqtt_screen_image", screen_image)
                cv2.waitKey(1)
            
        if screen_image is None:
            return
        

        debug = False
        if debug:
            cv2.imshow("origin_from_mqtt",  screen_image)
            cv2.waitKey(1)

        if self.screen_image_to_mqtt:
            self.__kvm_node.publish(screen_image)
        if self.screen_image_to_app_window_idendifier:
            pass
        if self.screen_image_to_image_divder:
            self.__divider.DivideToSmallImages(screen_image)

        if self.screen_image_to_ocr:
            strings = pytesseract.image_to_string(screen_image)
            Logger.Print("===============================================", strings)
        
        if self.small_images_to_ocr:
            Logger.Debug('ocr small images')
            for image in small_images:
                string = pytesseract.image_to_string(image)
                small_strings.append(string)
                Logger.Print('', string)
        if self.small_strings_to_mqtt:
            g_mqtt.publish("small_strings", small_strings)
