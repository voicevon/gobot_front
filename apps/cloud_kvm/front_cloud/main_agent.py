
from ocr_window import OcrWindow
from von.remote_var_mqtt import RemoteVar_mqtt, g_mqtt, g_mqtt_broker_config
from von.logger import Logger



if __name__ == "__main__":
    # g_mqtt_broker_config.client_id = "230502"
    # g_mqtt.connect_to_broker(g_mqtt_broker_config)

    ocr_window =  OcrWindow(99, "template_images/youtube.png")
    while True:
        ocr_window.SpinOnce()

        

