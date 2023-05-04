from von.mqtt_agent import  MqttAgent, g_mqtt, g_mqtt_broker_config
from von.remote_var_mqtt import RemoteVar_mqtt
from von.logger import Logger
import cv2
import numpy
import time



MARKING_STATE_IDLE = 0
MARKING_STATE_BEGIN = 1
MARKING_STATE_MOVE = 2
MARKING_STATE_ENDING = 3


origin_image = None
x1,y1, x2, y2 = 0,0,0,0
marking_state = MARKING_STATE_IDLE


# https://www.youtube.com/watch?v=2WR3wMt6V2k

def on_mouse_event(event, x, y, flags, param):
    global origin_image
    global x1,y1,x2,y2
    global marking_state
    if origin_image is None:
        return
    

    if event == cv2.EVENT_LBUTTONDOWN:
        if marking_state == MARKING_STATE_IDLE:
            x1,y1 = x, y
            marking_state = MARKING_STATE_MOVE
            
        elif marking_state == MARKING_STATE_MOVE:
            x2,y2 = x, y
            marking_state = MARKING_STATE_ENDING

            # save x1,y1,x2,y2
            
            marking_state = MARKING_STATE_IDLE
    
    if event == cv2.EVENT_MOUSEMOVE:
        if marking_state == MARKING_STATE_MOVE:
            x2,y2 = x,y
        Logger.Print("x1,y1, x2,y2", (x1,y1,x2,y2))
    
    marker = origin_image.copy()
    cv2.rectangle(marker, (x1,y1), (x2,y2), color=(255,0,0), thickness=2)
    cv2.imshow("marker", marker)
    cv2.waitKey(1)


    



if __name__ == '__main__':
    g_mqtt_broker_config.client_id = '23050a'
    g_mqtt.connect_to_broker(g_mqtt_broker_config)
    bytes_img =  RemoteVar_mqtt("twh/221109/kvm/screen", None)

    refresh_origin = True
    has_set_callback = False
    while True:
        if refresh_origin:
            if bytes_img.get() is not None:
                np_array = numpy.frombuffer(bytes_img.get(), dtype=numpy.uint8) 
                origin_image = cv2.imdecode(np_array, flags=1)
                cv2.imshow("origin", origin_image)
                if not has_set_callback:
                    cv2.setMouseCallback('origin', on_mouse_event)
                    has_set_callback = True
        
        key = cv2.waitKey(1)
        if key == ord(' '):
            # start/stop refresh
            refresh_origin = not refresh_origin
        if key == ord('2'):
            # mark template position and area
            refresh_origin = False
        time.sleep(0.05)


            





