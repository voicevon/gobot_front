from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
from von.logger import Logger
import cv2
import json

# https://www.youtube.com/watch?v=2WR3wMt6V2k

  
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
        self.__config_getter = RemoteVar_mqtt("ocr/" + app_window_name + "/config", None, for_loading_config=True)
        self.marking_state = MARKING_STATE_IDLE
        
        self.__app_window_config, _ = self.__config_getter.get_json()
        self.__marking_areas = self.__app_window_config["areas"]
        # Logger.Print("", self.__app_window_config)
        # Logger.Print("", self.__marking_areas)
        # for a in self.__marking_areas:
        #     Logger.Print('', a)
        self.__current_marking_area = self.__marking_areas[0]


    def __on_mouse_event(self, event, x, y, flags, param):
        if self.screen_image is None:
            return

        if event == cv2.EVENT_LBUTTONDOWN:
            if self.marking_state == MARKING_STATE_IDLE:
                # x1,y1 = x, y
                self.__current_marking_area['x1'] = x
                self.__current_marking_area['y1'] = y
                self.marking_state = MARKING_STATE_MOVE
                
            elif self.marking_state == MARKING_STATE_MOVE:
                # x2,y2 = x, y
                self.__current_marking_area['x2'] = x
                self.__current_marking_area['y2'] = y 
                self.marking_state = MARKING_STATE_ENDING

                self.marking_state = MARKING_STATE_IDLE
        
        if event == cv2.EVENT_MOUSEMOVE:
            if self.marking_state == MARKING_STATE_MOVE:
                self.__current_marking_area['x2'] = x
                self.__current_marking_area['y2'] = y 
            # Logger.Print("x1,y1, x2,y2", (x1,y1,x2,y2))
        
    def draw_rectangles(self):
        screen_image_with_rectangles = self.screen_image.copy()
        # draw all rectangle of areas, current working area is green, others are red.
        for area in self.__marking_areas:
            x1 = area['x1']
            y1 = area['y1']
            x2 = area['x2']
            y2 = area['y2']
            if area == self.__current_marking_area:
                color = (0, 255, 0)
            else:
                color = (0, 0, 255)
            cv2.rectangle(screen_image_with_rectangles, (x1, y1),(x2,y2), color, thickness=2)
        return screen_image_with_rectangles
        

    def SpinOnce(self, screen_image):
        if screen_image is not None:
            self.screen_image = screen_image
            if self.refresh_origin:
                if not self.has_set_callback:
                    cv2.imshow(self.__screen_image_cv_window_name, screen_image)
                    cv2.waitKey(1)
                    cv2.setMouseCallback(self.__screen_image_cv_window_name, self.__on_mouse_event)
                    self.has_set_callback = True

        if self.screen_image is not None:
            rectangle_image=  self.draw_rectangles()
            cv2.imshow(self.__screen_image_cv_window_name, rectangle_image)

        key = cv2.waitKey(1)
        if key == ord(' '):
            # start/stop refresh
            self.refresh_origin = not self.refresh_origin
            if not self.refresh_origin:
                Logger.Info("Stop auto refreshing source image")
        if key == ord('1'):
            self.__current_marking_area = self.__marking_areas[1]
            Logger.Print("marking_id", 1)
        if key == ord('2'):
            self.__current_marking_area = self.__marking_areas[2]
            Logger.Print("marking_id", 2)
        if key == ord('3'):
            self.__current_marking_area = self.__marking_areas[3]
            Logger.Print("marking_id", 3)
        if key == ord('4'):
            self.__current_marking_area = self.__marking_areas[4]
            Logger.Print("marking_id", 4)
        if key == ord('5'):
            self.__current_marking_area = self.__marking_areas[5]
            Logger.Print("marking_id", 5)
        if key == ord('6'):
            self.__current_marking_area = self.__marking_areas[6]
            Logger.Print("marking_id", 6)
        if key == ord('7'):
            self.__current_marking_area = self.__marking_areas[7]
            Logger.Print("marking_id", 7)
        if key == ord('8'):
            self.__current_marking_area = self.__marking_areas[8]
            Logger.Print("marking_id", 8)
        if key == ord('9'):
            self.__current_marking_area = self.__marking_areas[9]
            Logger.Print("marking_id", 9)
        if key == ord('s'):
            self.__save_to_mqtt()
            Logger.Info("updated areas")

    def __save_to_mqtt(self):
        payload = json.dumps(self.__app_window_config)
        self.__config_getter.set(payload)





            
