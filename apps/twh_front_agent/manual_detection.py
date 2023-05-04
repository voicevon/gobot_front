from von.mqtt_agent import  MqttAgent, g_mqtt, g_mqtt_broker_config
from von.remote_var_mqtt import RemoteVar_mqtt
from von.logger import Logger
import cv2
import numpy
import time
import json


class SingleMarker:
    def __init__(self, id:int) -> None:
        self.id = id
        self.x1 = 1
        self.y1 = 2
        self.x2 = 3
        self.y2 = 4

    # def get_elements(self):
    #     return self.x1, self.y1, self.x2, self.y2

    def update_position(self, x1,y1,x2,y2):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2

    def draw_rectangle(self, image, color):
        cv2.rectangle(image, (self.x1, self.y1), (self.x2, self.y2), color, thickness=2)

    def get_json(self) :
        result = {}
        result["id"] = self.id
        result["postions"] = self.x1, self.y1, self.x2, self.y2
        return result


MARKING_STATE_IDLE = 0
MARKING_STATE_BEGIN = 1
MARKING_STATE_MOVE = 2
MARKING_STATE_ENDING = 3


origin_image = None
x1,y1, x2, y2 = 0,0,0,0
marking_state = MARKING_STATE_IDLE
marking_id = 1
mark_areas = []
for i in range(10):
    new_marker = SingleMarker(i)
    mark_areas.append(new_marker)
print(mark_areas)

# https://www.youtube.com/watch?v=2WR3wMt6V2k

def on_mouse_event(event, x, y, flags, param):
    global origin_image
    global x1,y1,x2,y2
    global marking_state
    global mark_areas
    global marking_id

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
            mark_areas[marking_id].update_position(x1,y1,x2,y2)
            
            marking_state = MARKING_STATE_IDLE
    
    if event == cv2.EVENT_MOUSEMOVE:
        if marking_state == MARKING_STATE_MOVE:
            x2,y2 = x,y
        # Logger.Print("x1,y1, x2,y2", (x1,y1,x2,y2))
    
def redraw_areas():
    global origin_image
    global mark_areas


    marker = origin_image.copy()
    for area in mark_areas:
        # xx1,xx2,yy1,yy2 = area.get_elements()
        # cv2.rectangle(marker, (xx1,yy1), (xx2,yy2), color=(255,0,0), thickness=2)
        if marking_id == area.id:
            color = (255,0,0)
        else:
            color = (0,0,255)
        area.draw_rectangle(marker, color)

    cv2.imshow("marker", marker)
    cv2.waitKey(1)

def get_positions_json() :
    res = {}
    res["name"] = "abcde"
    res["areas"] = []
    for area in mark_areas:
        res["areas"].append(area.get_json())
    
    return res



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

        if origin_image is not None:
            redraw_areas()

        key = cv2.waitKey(100)
        if key == ord(' '):
            # start/stop refresh
            refresh_origin = not refresh_origin
        if key == ord('1'):
            marking_id = 1
            Logger.Print("marking_id", 1)
        if key == ord('2'):
            marking_id = 2
            Logger.Print("marking_id", 2)
        if key == ord('3'):
            marking_id = 3
            Logger.Print("marking_id", 3)
        if key == ord('4'):
            marking_id = 4
            Logger.Print("marking_id", 4)
        if key == ord('5'):
            marking_id = 5
            Logger.Print("marking_id", 5)
        if key == ord('6'):
            marking_id = 6
            Logger.Print("marking_id", 6)
        if key == ord('7'):
            marking_id = 7
            Logger.Print("marking_id", 7)
        if key == ord('8'):
            marking_id = 8
            Logger.Print("marking_id", 8)
        if key == ord('9'):
            marking_id = 9
            Logger.Print("marking_id", 9)
        if key == ord('s'):
            result = get_positions_json()
            # Logger.Print('json', result)
            payload = json.dumps(result)
            g_mqtt.publish("kvm/marks/test", payload)

        # time.sleep(0.05)


            





