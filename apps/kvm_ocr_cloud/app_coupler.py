
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt, g_mqtt, g_mqtt_broker_config
from von.logger import Logger
from von.ocr.ocr_window import OcrWindow
from libs.kvm_node import KvmNode




class OcrCoupler:
    def __init__(self, kvm_node_name:str, app_window_name:str) -> None:
        self.__image_getter = None
        if kvm_node_name == 'local_kvm_node':
            kvm_node_config_getter = RemoteVar_mqtt('ocr/kvm/' + kvm_node_name + '/config', None)
            while not kvm_node_config_getter.rx_buffer_has_been_updated():
                pass
            kvm_node_config = kvm_node_config_getter.get_json()
            self.__kvm_node = KvmNode(kvm_node_name, kvm_node_config)
        else:
            self.__image_getter = RemoteVar_mqtt("ocr/kvm/" + kvm_node_name + "/screen_image", None)
        self.__ocr_window = OcrWindow(app_window_name)

    def SpinOnce(self):
        if self.__image_getter is None:
            screen_image = self.__kvm_node.Capture_Camera()
        else:
            while not self.__image_getter.rx_buffer_has_been_updated():
                pass
            screen_image = self.__image_getter.get_cv_image()

        self.__ocr_window.SpinOnce(screen_image)





if __name__ == "__main__":
    g_mqtt_broker_config.client_id = "230502"
    g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)
    

    # ocr_window =  OcrFactory.CreateOcrWindow(kvm_node_name="kvm_230506", app_window_name= "ubuntu_performance")
    # ocr_window = OcrWindow("kvm_230506",'ubuntu_performance', True)
    ocr_coupler = OcrCoupler("kvm_230506",'ubuntu_performance')
    while True:
        ocr_coupler.SpinOnce()

        

