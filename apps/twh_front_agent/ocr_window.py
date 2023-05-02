import cv2
import numpy as np  # pip install numpy
from mss import mss      # pip install mss
from PIL import Image #pip install pillow


from ocr_unit_factory import OcrUnitFactory, OcrUnit



class OcrWindow:
    
    def __init__(self, camera_id, template_filename:str) -> None:
        if (camera_id == 99):
            # capture from screen
            pass
        else:
            self.__cap = cv2.VideoCapture(camera_id)

        self.__camer_id = camera_id
        self.__template_image = cv2.imread(template_filename)

        self.__all_units = []
        self.__all_units.append(OcrUnitFactory.CreateOcrUnit('title'))



    

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
            cv2.imshow('output',origin)
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
        frame = None
        if self.__camer_id == 99:
            frame = self.__capture_screen()
        else:
            ret, frame = self.__cap.read()

        if frame is not None:
            cv2.imshow('origin', frame)
            cv2.waitKey(1)

        # Now we got a frame, try to  find marker.  
        left, top = self.match_template(frame)

        # crop frame to many ocr units, base location is the marker.
        for unit in self.__all_units:
            unit.


