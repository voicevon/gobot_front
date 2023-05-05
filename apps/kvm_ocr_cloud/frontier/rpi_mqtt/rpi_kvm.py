import cv2
from von.mqtt_agent import g_mqtt, g_mqtt_broker_config
from von.logger import Logger
from von.remote_var_mqtt import RemoteVar_mqtt

import time

os = 'Windows'
os = 'Linux_desktop'
os = 'Pi_lite'


if __name__ == '__main__':
    if os == 'Windows':
        cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
    else:
        cap = cv2.VideoCapture(0, cv2.CAP_V4L2)

    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

    # https://stackoverflow.com/questions/52068277/change-frame-rate-in-opencv-3-4-2
    # cap.set(cv2.CAP_PROP_FPS, 10)
    # fps = int(cap.get(5))
    # print("fps:", fps)

    g_mqtt_broker_config.client_id = "230504"
    g_mqtt.connect_to_broker(g_mqtt_broker_config)

    fps_limit = RemoteVar_mqtt("ocr/ubuntu_performance/kvm_config", 1)
    Logger.Print("fps_limit", fps_limit.get())
    publish_counter = 0
    start_time = 0
    while True:
        ret, frame = cap.read()
        if ret:
            now_time = time.time()
            if int(now_time - start_time) > int(fps_limit.get()):
                if os !='Pi_lite':
                    cv2.imshow('camera', frame)
                    cv2.waitKey(1)
                g_mqtt.publish_cv_image("ocr/ubuntu_performance/screen_image",  frame)
                start_time = time.time()

                publish_counter += 1
                Logger.Print("publish counter", publish_counter)


