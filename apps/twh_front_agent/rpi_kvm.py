import cv2
from von.mqtt_agent import g_mqtt, g_mqtt_broker_config, MqttAgent
import time


if __name__ == '__main__':
    cap = cv2.VideoCapture(0)
    g_mqtt_broker_config.client_id = "230803"
    g_mqtt.connect_to_broker(g_mqtt_broker_config)


    while True:
        ret, frame = cap.read()
        if ret:
            g_mqtt.publish_cv_image("twh/221109/kvm/screen",  frame)
            time.sleep(10)


