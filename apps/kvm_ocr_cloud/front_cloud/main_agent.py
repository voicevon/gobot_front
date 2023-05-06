
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt, g_mqtt, g_mqtt_broker_config
from von.logger import Logger
# from von.ocr.ocr_factory import OcrFactory
from von.ocr.ocr_window import OcrWindow




if __name__ == "__main__":
    g_mqtt_broker_config.client_id = "230502"
    g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)
    

    # ocr_window =  OcrFactory.CreateOcrWindow(kvm_node_name="kvm_230506", app_window_name= "ubuntu_performance")
    # ocr_window = OcrWindow("kvm_230506",'ubuntu_performance', True)
    ocr_window = OcrWindow("kvm_230506",'ubuntu_performance')
    while True:
        ocr_window.SpinOnce()

        

