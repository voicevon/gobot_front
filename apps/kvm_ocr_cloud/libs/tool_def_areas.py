from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
from von.logger import Logger
import cv2
import json

# https://www.youtube.com/watch?v=2WR3wMt6V2k

class AreaMarker:

    def __init__(self, id:int) -> None:
        self.id = id
        self.x1 = 1
        self.y1 = 2
        self.x2 = 3
        self.y2 = 4

    def update_position(self, x1,y1,x2,y2):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2

    def draw_rectangle(self, image, color, area):
        cv2.rectangle(image, (self.x1, self.y1), (self.x2, self.y2), color, thickness=2)

    # def get_json(self) :
    #     result = {}
    #     result["id"] = self.id
    #     result["postions"] = self.x1, self.y1, self.x2, self.y2
    #     return result
      
MARKING_STATE_IDLE = 0
MARKING_STATE_BEGIN = 1
MARKING_STATE_MOVE = 2
MARKING_STATE_ENDING = 3

class ToolDefAreas:

    def __init__(self, app_window_name) -> None:
        self.screen_image = None
        self.__screen_image_cv_window_name = "ToolDefAreas::" + app_window_name
        self.refresh_origin = True
        self.has_set_callback = False
        self.areas=[]
        self.__config_getter = RemoteVar_mqtt("ocr/" + app_window_name + "/config", None, for_loading_config=True)
        self.marking_state = MARKING_STATE_IDLE
        
        self.__app_window_config, _ = self.__config_getter.get_json()
        Logger.Print("", self.__app_window_config)
        self.__mark_areas = self.__app_window_config["areas"]
        self.__current_marking_area = self.__mark_areas[0]

    def __on_mouse_event(self, event, x, y, flags, param):
        if self.screen_image is None:
            return

        if event == cv2.EVENT_LBUTTONDOWN:
            if self.marking_state == MARKING_STATE_IDLE:
                # x1,y1 = x, y
                self.__current_marking_area.x1 = x
                self.__current_marking_area.y1 = y
                self.marking_state = MARKING_STATE_MOVE
                
            elif self.marking_state == MARKING_STATE_MOVE:
                # x2,y2 = x, y
                self.__current_marking_area.x2 = x
                self.__current_marking_area.y2 = y 
                self.marking_state = MARKING_STATE_ENDING

                # save x1,y1,x2,y2
                
                # self.__current_marking_area.update_position(x1,y1,x2,y2)
                
                self.marking_state = MARKING_STATE_IDLE
        
        if event == cv2.EVENT_MOUSEMOVE:
            if self.marking_state == MARKING_STATE_MOVE:
                x2,y2 = x,y
            # Logger.Print("x1,y1, x2,y2", (x1,y1,x2,y2))
        
    def redraw_areas(self):
        if self.screen_image is None:
            return
        
        marker = self.screen_image.copy()
        area_marker = AreaMarker(0)

        for area in self.__mark_areas:
            # xx1,xx2,yy1,yy2 = area.get_elements()
            # cv2.rectangle(marker, (xx1,yy1), (xx2,yy2), color=(255,0,0), thickness=2)
            if area == self.__current_marking_area:
                color = (255,0,0)
            else:
                color = (0,0,255)
            area_marker.draw_rectangle(marker, color, area)

        cv2.imshow("marker", marker)

    def SpinOnce(self, screen_image):
        if screen_image is not None:
            self.screen_image = screen_image
            if self.refresh_origin:
                if not self.has_set_callback:
                    debug = True
                    if debug:
                        cv2.imshow(self.__screen_image_cv_window_name, screen_image)
                        cv2.waitKey(1)
                    cv2.setMouseCallback(self.__screen_image_cv_window_name, self.__on_mouse_event)
                    self.has_set_callback = True

        self.redraw_areas()
        key = cv2.waitKey(1)
        if key == ord(' '):
            # start/stop refresh
            self.refresh_origin = not self.refresh_origin
            if not self.refresh_origin:
                Logger.Info("Stop auto refreshing source image")
        if key == ord('1'):
            self.__current_marking_area = self.__mark_areas[1]
            Logger.Print("marking_id", 1)
        if key == ord('2'):
            self.__current_marking_area = self.__mark_areas[2]
            Logger.Print("marking_id", 2)
        if key == ord('3'):
            self.__current_marking_area = self.__mark_areas[3]
            Logger.Print("marking_id", 3)
        if key == ord('4'):
            self.__current_marking_area = self.__mark_areas[4]
            Logger.Print("marking_id", 4)
        if key == ord('5'):
            self.__current_marking_area = self.__mark_areas[5]
            Logger.Print("marking_id", 5)
        if key == ord('6'):
            self.__current_marking_area = self.__mark_areas[6]
            Logger.Print("marking_id", 6)
        if key == ord('7'):
            self.__current_marking_area = self.__mark_areas[7]
            Logger.Print("marking_id", 7)
        if key == ord('8'):
            self.__current_marking_area = self.__mark_areas[8]
            Logger.Print("marking_id", 8)
        if key == ord('9'):
            self.__current_marking_area = self.__mark_areas[9]
            Logger.Print("marking_id", 9)
        if key == ord('s'):
            self.__save_to_mqtt()
            Logger.Info("updated areas")

    def __save_to_mqtt(self):
        payload = json.dumps(self.__app_window_config)
        self.__config_getter.set(payload)





            
