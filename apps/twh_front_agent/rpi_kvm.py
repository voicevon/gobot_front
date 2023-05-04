import cv2
from von.mqtt_agent import g_mqtt, g_mqtt_broker_config
from von.logger import Logger
from von.remote_var_mqtt import RemoteVar_mqtt

import time

if __name__ == '__main__':
    cap = cv2.VideoCapture(0)
    g_mqtt_broker_config.client_id = "230504"
    g_mqtt.connect_to_broker(g_mqtt_broker_config)

    interal_seconds = RemoteVar_mqtt("twh/221109/kvm/config/interval_second", 1)
    publish_counter = 0

    while True:
        ret, frame = cap.read()
        if ret:
            g_mqtt.publish_cv_image("twh/221109/kvm/screen",  frame)
            time.sleep(interal_seconds.get())
            Logger.Print("publish counter", publish_counter)


