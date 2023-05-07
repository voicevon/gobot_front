
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt, g_mqtt, g_mqtt_broker_config
from von.logger import Logger
from libs.ocr_window import OcrWindow
from libs.kvm_node_camera import KvmNodeCamera
from libs.kvm_node_screen import KvmNodeScreen




class OcrCoupler:
    def __init__(self, kvm_node_name:str, app_window_name:str) -> None:
        '''

        '''
        self.__image_getter = None
        if kvm_node_name == 'local_kvm_node':
            kvm_node_config_getter = RemoteVar_mqtt('ocr/kvm/' + kvm_node_name + '/config', None, True)
            self.__kvm_node = KvmNodeCamera(kvm_node_name, kvm_node_config_getter)
        else:
            self.__image_getter = RemoteVar_mqtt("ocr/kvm/" + kvm_node_name + "/screen_image", None, True)
        self.__ocr_window = OcrWindow(app_window_name)

    def SpinOnce(self):
        if self.__image_getter is None:
            screen_image = self.__kvm_node.Capture_Image()
            self.__ocr_window.SpinOnce(screen_image)
        else:
            screen_image, has_been_updated = self.__image_getter.get_cv_image()
            if has_been_updated:
                self.__ocr_window.SpinOnce(screen_image)
        


if __name__ == "__main__":
    g_mqtt_broker_config.client_id = "230502cfg"
    g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)

    ocr_coupler = OcrCoupler("kvm_230506",'ubuntu_performance')
    while True:
        ocr_coupler.SpinOnce()

        

