from von.mqtt.remote_var_mqtt import RemoteVar_mqtt,g_mqtt,g_mqtt_broker_config
from von.logger import Logger
from libs.kvm_node_camera import KvmNodeCamera
from libs.screen_image_divider import ScreenImageDivider


# my_os = 'Pi_lite'

class OcrNode:
    def __init__(self, kvm_node_name:str, app_window_name:str, os_name:str) -> None:

        getter = RemoteVar_mqtt('ocr/kvm/' + kvm_node_name + '/config', None, for_loading_config=True)
        self.__kvm_node = KvmNodeCamera(os_name, getter)

        getter =  RemoteVar_mqtt('ocr/' + app_window_name + '/config', None, for_loading_config=True)
        self.__divder = ScreenImageDivider(app_window_name)
        
        self.from_screen_capture = False
        self.from_camera_capture = False
        self.from_mqtt_broker = False
        self.screen_image_to_mqtt_ = False
        self.screen_image_to_app_window_idendifier = False
        self.screen_image_to_image_divder = False
        self.screen_image_to_ocr = False
        self.small_images_to_ocr = False
        
    def SpinOnce(self):
        screen_image = None
        if self.from_camera_capture:
            screen_image = self.__kvm_node.Capture_Image()
        if self.from_screen_capture:
            pass
        if self.from_mqtt_broker:
            pass
        if screen_image is None:
            # TODO: warning
            Logger.Error('OcrNode::SpinOnce()  no source of screen image')
            return
        else:
            if self.to_mqtt_:
                self.__kvm_node.publish(screen_image)
            if self.to_app_window_idendifier:
                pass
            if self.to_image_divder:
                pass
            if self.to_ocr:
                pass
        


if __name__ == "__main__":

    g_mqtt_broker_config.client_id = "230502cfg"
    g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)

    kvm_node_name = 'kvm230506'
    app_window_name = 'ubuntu_performance'
    my_os = 'Windows'

    ocr_node = OcrNode(kvm_node_name, '', my_os) 
    # ocr_node = OcrNode('', app_window_name, my_os)
    # ocr_node = OcrNode(kvm_node_name, app_window_name, my_os)

    while True:
        ocr_node.SpinOnce()