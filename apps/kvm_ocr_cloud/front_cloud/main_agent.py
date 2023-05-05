
from von.remote_var_mqtt import RemoteVar_mqtt, g_mqtt, g_mqtt_broker_config
from von.logger import Logger
from ocr_factory import OcrFactory




if __name__ == "__main__":
    g_mqtt_broker_config.client_id = "230502"
    g_mqtt.connect_to_broker(g_mqtt_broker_config)

    ocr_window =  OcrFactory.CreateOcrWindow("ubuntu_performance")
    while True:
        ocr_window.SpinOnce()

        

