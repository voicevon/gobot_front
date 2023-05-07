from von.mqtt.remote_var_mqtt import RemoteVar_mqtt,g_mqtt,g_mqtt_broker_config
from von.logger import Logger
from libs.kvm_node_camera import KvmNodeCamera
from libs.app_window_idenifier import AppWindowIdentifier
from libs.screen_image_divider import ScreenImageDivider
import pytesseract
import cv2

# my_os = 'Pi_lite'
# class ImageRouter:


class OcrNode:
    def __init__(self, kvm_node_name:str, app_window_name:str, os_name:str) -> None:
        getter = RemoteVar_mqtt('ocr/kvm/' + kvm_node_name + '/config', None, for_loading_config=True)
        self.__kvm_node = KvmNodeCamera(os_name, getter)
        self.__image_getter = RemoteVar_mqtt(self.__kvm_node.mqtt_topic_of_screen_image, None)


        self.__identifier = AppWindowIdentifier()
        Logger.Print("OcrNode::init()  point 21","")
        getter =  RemoteVar_mqtt('ocr/' + app_window_name + '/config', None, for_loading_config=True)
        Logger.Print("OcrNode::init()  point 22","")
        self.__divder = ScreenImageDivider(app_window_name)
        Logger.Print("OcrNode::init()  point 23","")
        
        self.from_screen_capture = False
        self.from_camera_capture = False
        self.from_mqtt_broker = False
        self.screen_image_to_mqtt_ = False
        self.screen_image_to_app_window_idendifier = False
        self.screen_image_to_image_divder = False
        self.screen_image_to_ocr = False
        self.small_images_to_ocr = False
        self.small_strings_to_mqtt = False

        
    def SpinOnce(self):
        screen_image = None
        small_images = []
        small_strings=[]

        if self.from_camera_capture:
            screen_image = self.__kvm_node.Capture_Image()
        if self.from_screen_capture:
            pass
        if self.from_mqtt_broker:
            if self.__image_getter.rx_buffer_has_been_updated():
                screen_image, _ = self.__image_getter.get_cv_image()
                cv2.imshow("mqtt_screen_image", screen_image)
                cv2.waitKey(1)
            
        if screen_image is None:
            return
        
        
        if self.screen_image_to_mqtt_:
            self.__kvm_node.publish(screen_image)
        if self.screen_image_to_app_window_idendifier:
            pass
        if self.screen_image_to_image_divder:
            self.__divder.DivideToSmallImages(screen_image)

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



if __name__ == "__main__":

    g_mqtt_broker_config.client_id = "230502cfg"
    g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)

    kvm_node_name = 'kvm_230506'
    app_window_name = 'ubuntu_performance'
    my_os = 'Windows'

    ocr_node = OcrNode(kvm_node_name, app_window_name, my_os) 
    ocr_node.from_mqtt_broker = True
    # ocr_node.screen_image_to_ocr = True
    ocr_node.screen_image_to_image_divder = True
    # ocr_node = OcrNode('', app_window_name, my_os)
    # ocr_node = OcrNode(kvm_node_name, app_window_name, my_os)

    while True:
        ocr_node.SpinOnce()