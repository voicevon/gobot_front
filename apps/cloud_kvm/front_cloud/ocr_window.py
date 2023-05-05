from ocr_factory import OcrFactory, OcrUnit
from von.remote_var_mqtt import RemoteVar_mqtt

import cv2
import numpy as np  # pip install numpy
from mss import mss      # pip install mss
from PIL import Image #pip install pillow



class OcrWindow:

    # def __init__(self, camera_id, template_filename:str) -> None:
    def __init__(self, mqtt_topic_of_config:str, mqtt_topic_of_image) -> None:
        '''
        Image source is mqtt-message
        '''
        # if (camera_id == 99):
        #     # capture from screen
        #     pass
        # else:
        #     self.__cap = cv2.VideoCapture(camera_id)

        # self.__camer_id = camera_id
        # self.__template_image = cv2.imread(template_filename)
        self.__config = RemoteVar_mqtt(mqtt_topic_of_config, {})
        self.__image = RemoteVar_mqtt(mqtt_topic_of_image, None)
        self.__is_loaded = False  # Load image and config, backgroundly

        self.__all_units = []
        self.__frame = None


    def __load_config(self):
        if self.__config.rx_buffer_has_been_renewed():
            self.__all_units.append(OcrFactory.CreateOcrUnit('title'))
            self.__is_loaded = True

    # function to display the coordinates of
    # of the points clicked on the image
    def click_event(self, event, x, y, flags, params):
        if self.__frame is None:
            return
        
        # checking for left mouse clicks
        if event == cv2.EVENT_LBUTTONDOWN:

            # displaying the coordinates
            # on the Shell
            print(x, ' ', y)

            # displaying the coordinates
            # on the image window
            font = cv2.FONT_HERSHEY_SIMPLEX
            cv2.putText(self.__frame, str(x) + ',' +
                        str(y), (x,y), font,
                        1, (255, 0, 255), 2)
            cv2.imshow('origin', self.__frame)
            cv2.waitKey(2000)

        # checking for right mouse clicks	
        if event==cv2.EVENT_RBUTTONDOWN:

            # displaying the coordinates
            # on the Shell
            print(x, ' ', y)

            # displaying the coordinates
            # on the image window
            font = cv2.FONT_HERSHEY_SIMPLEX
            b = self.__frame[y, x, 0]
            g = self.__frame[y, x, 1]
            r = self.__frame[y, x, 2]
            cv2.putText(self.__frame, str(b) + ',' +
                        str(g) + ',' + str(r),
                        (x,y), font, 1,
                        (255, 255, 0), 2)
            cv2.imshow('origin', self.__frame)
            cv2.waitKey(2000)

    def match_template(self, origin):
        method = cv2.TM_SQDIFF_NORMED

        result = cv2.matchTemplate(self.__template_image, origin, method)
        # We want the minimum squared difference
        mn,_,mnLoc,_ = cv2.minMaxLoc(result)
        # Draw the rectangle:
        # Extract the coordinates of our best match
        MPx, MPy = mnLoc

        imshow = False
        if imshow:
            # Step 2: Get the size of the template. This is the same size as the match.
            trows, tcols = self.__template_image.shape[:2]
            # Step 3: Draw the rectangle on origin
            cv2.rectangle(origin, (MPx,MPy),(MPx+tcols,MPy+trows),(0,0,255),2)
            cv2.imshow('match template',origin)
            cv2.waitKey(1)
        return MPx, MPy
    
    def __capture_screen(self):
            area = {'left': 0, 'top': 0, 'width': 1366, 'height': 768}
            with mss() as sct:
                screenShot = sct.grab(area)
                mss_img = Image.frombytes('RGB', (screenShot.width, screenShot.height), screenShot.rgb)
                # img = Image.frombytes('P', (screenShot.width, screenShot.height), screenShot.rgb)

                cv_img = np.array(mss_img)
                frame = cv2.cvtColor(cv_img, cv2.COLOR_BGR2RGB)
                return frame
            
    def SpinOnce(self):
        if self.__is_loaded:
            self.__frame = self.__image.get()
            # Now we got a frame, try to  find marker.  
            left, top = self.match_template(self.__frame)

            # crop frame to many ocr units, base location is the marker.
            for unit in self.__all_units:
                image = unit.GetAreaImage(self.__frame, left, top)
                cv2.imshow(unit.name, image)
        else:
            self.__load_config()



