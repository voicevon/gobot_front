from von.mqtt.mqtt_agent import  g_mqtt, g_mqtt_broker_config
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
from von.logger import Logger
import cv2
import numpy
from libs.crop_area import SingleMarker
from libs.ocr_factory import OcrFactory
from libs.ocr_window import OcrWindow



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

def get_positions_json() :
    res = {}
    res["name"] = "abcde"
    res["areas"] = []
    for area in mark_areas:
        res["areas"].append(area.get_json())
    
    return res["areas"]


# marking_id_keys = {"1":1,"2":2,"3":3,"4":4,"5":5,"6":6,"7":7,"8":8,"9":9 }

if __name__ == '__main__':
    g_mqtt_broker_config.client_id = '23050a'
    g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)
    kvm_node_name = 'kvm_230506'
    Logger.Print("main  point 31", '')
    kvm_node_config =  OcrFactory.CreateKvmNodeConfig(kvm_node_name)
    Logger.Print("main  point 32", '')
    mqtt_topic_of_screen_image = kvm_node_config["topic_of_screen_image"]
    Logger.Print("main  point 33", '')

    app_window_name = 'ubuntu_performance'
    # ocr_window = OcrFactory.CreateOcrWindow(kvm_node_name= "nothing", app_window_name= app_window_name)
    ocr_window = OcrWindow(app_window_name)
    Logger.Print("main  point 34", '')
    imgage_getter =  RemoteVar_mqtt(mqtt_topic_of_screen_image , None)
    Logger.Print("main  point 35", '')

    refresh_origin = True
    has_set_callback = False
    while True:
        if refresh_origin:
            if imgage_getter.rx_buffer_has_been_updated():
                origin_image = imgage_getter.get_cv_image()
                cv2.imshow("origin", origin_image)
                if not has_set_callback:
                    cv2.setMouseCallback('origin', on_mouse_event)
                    has_set_callback = True

        if origin_image is not None:
            redraw_areas()

        key = cv2.waitKey(1)

        # if key in marking_id_keys.keys():
        #     marking_id = marking_id_keys[key]
        #     Logger.Print("from diction,    marking_id", marking_id)

        if key == ord(' '):
            # start/stop refresh
            refresh_origin = not refresh_origin
            if not refresh_origin:
                Logger.Info("Stop auto refreshing source image")
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
            areas = get_positions_json()
            ocr_window.update_areas(areas)
            Logger.Info("updated areas")

        # time.sleep(0.05)


            





