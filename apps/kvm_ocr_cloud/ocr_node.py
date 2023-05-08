from von.logger import Logger
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt, g_mqtt
from libs.kvm_node_camera import KvmNodeCamera
from libs.kvm_node_screen import KvmNodeScreen
from libs.app_window_idenifier import AppWindowIdentifier
from libs.screen_image_divider import ScreenImageDivider
from libs.tool_def_areas import ToolDefAreas
import pytesseract
import cv2
from PIL import Image #pip install pillow


class OcrNode:
    
    def __init__(self, routing) -> None:
        self.routing = routing
        #basic environment
        self.__kvm_node_name = routing['kvm_node_name']
        self.__app_window_name = routing['app_window_name']
        self.__my_os = routing['my_os']

        # screen_image comes from where
        self.from_screen_capture = routing['from_screen_capture']
        self.from_camera_capture = routing['from_camera_capture']
        self.from_mqtt = routing['from_mqtt']

        # screen_images goes to where
        self.screen_image_to_mqtt = routing['screen_image_to_mqtt']
        self.screen_image_to_app_window_idendifier = routing['screen_image_to_app_window_identifier']
        self.screen_image_to_image_divder = routing['screen_image_to_image_divider']
        self.screen_image_to_ocr = routing['screen_image_to_ocr']
        self.screen_image_to_tool_areas_marker = routing['screen_image_to_tool_areas_marker']

        # small_images and small_string go to where
        self.small_images_to_ocr = routing['small_images_to_ocr']
        self.small_strings_to_mqtt = routing['small_strings_to_mqtt']


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
            config, _ = getter.get_json()
            Logger.Print("ffffffffffffffff", config)
            self.__divider = ScreenImageDivider(config['areas'])

        if self.screen_image_to_tool_areas_marker:
            self.__tool_areas_marker = ToolDefAreas(self.__app_window_name)

    def __get_screen_image(self):
        screen_image = None
        if self.from_camera_capture:
            screen_image = self.__kvm_node.Capture_Image()
        if self.from_screen_capture:
            screen_image = self.__kvm_node.Capture_Image()
        if self.from_mqtt:
            if self.__image_getter.rx_buffer_has_been_updated():
                screen_image, _ = self.__image_getter.get_cv_image()
                debug = False
                if debug:
                    cv2.imshow("OcrNode::mqtt_screen_image", screen_image)
                    cv2.waitKey(1)
            
        return screen_image

    def __deliver_screen_image_without_return(self, screen_image):
        if self.screen_image_to_mqtt:
            self.__kvm_node.publish(screen_image)

        if self.screen_image_to_ocr:
            strings = pytesseract.image_to_string(screen_image)
            Logger.Print("===============================================", strings)

    def __deliver_screen_image_even_is_none(self, screen_image):
        if self.screen_image_to_tool_areas_marker:
            self.__tool_areas_marker.SpinOnce(screen_image)

    def SpinOnce(self):
        small_images = []
        small_strings= []

        screen_image = self.__get_screen_image()
        self.__deliver_screen_image_even_is_none(screen_image)

        if screen_image is None:
            return
        
        debug = True
        if debug:
            cv2.imshow("OcrNode::SpinOnce().debug_screen_image",  screen_image)
            cv2.waitKey(1)
        
        self.__deliver_screen_image_without_return(screen_image)

        if self.screen_image_to_app_window_idendifier:
            app_window_name = self.__identifier.SpinOnce(screen_image)

        if self.screen_image_to_image_divder:
            small_images = self.__divider.DivideToSmallImages(screen_image)

        if self.small_images_to_ocr:
            # TODO:  do this at new thread.
            Logger.Debug('ocr small images')
            for image in small_images:
                color_converted = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                pil_image =  Image.fromarray(color_converted)
                string = pytesseract.image_to_string(pil_image)
                small_strings.append(string)


                # Logger.Print("pytesseract string", string)
                # Logger.Print('', string)

        if self.small_strings_to_mqtt:
            Logger.Debug('small_strings_to_mqtt')
            index = 0
            for string in small_strings:
                mqtt_topic = "ocr/" + self.__kvm_node_name + "/" + self.__app_window_name + "/" + str(index) + "/string"
                g_mqtt.publish(mqtt_topic, string)
                if index == 1:
                    try:
                        cpu =  int(string[0:2])
                        Logger.Print("cpu demo", cpu)
                        g_mqtt.publish("ocr/demo", cpu)
                    except:
                        pass
                index += 1
