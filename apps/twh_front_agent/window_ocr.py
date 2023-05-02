import cv2
import numpy as np  # pip install numpy
from mss import mss      # pip install mss
from PIL import Image #pip install pillow

class OcrUnit:
    def __init__(self) -> None:
        self.name = "new_target_area"
        self.template_filename = ""
        self.top_offset = 0
        self.left_offset = 0
        self.width = 100
        self.height = 100

    def LoadTemplateImageFromFile(self):
        self.template_image = cv2.imread(self.template_filename)

    def match_template(self, origin):
        method = cv2.TM_SQDIFF_NORMED

        result = cv2.matchTemplate(self.template_image, origin, method)
        # We want the minimum squared difference
        mn,_,mnLoc,_ = cv2.minMaxLoc(result)
        # Draw the rectangle:
        # Extract the coordinates of our best match
        MPx,MPy = mnLoc

        imshow = False
        if imshow:
            # Step 2: Get the size of the template. This is the same size as the match.
            trows,tcols = self.template_image.shape[:2]
            # Step 3: Draw the rectangle on origin
            cv2.rectangle(origin, (MPx,MPy),(MPx+tcols,MPy+trows),(0,0,255),2)
            cv2.imshow('output',origin)
            cv2.waitKey(1)
        return MPx, MPy

    def GetAreaImage(self, origin):
        # get_target_area(origin, left, top, width, height)
        left, top = self.match_template(origin)
        y1 = top + self.top_offset
        x1 = left + self.left_offset
        y2 = y1 + self.height
        x2 = x1 + self.width
        cropped_image = origin[y1:y2, x1:x2]
        return cropped_image

class OcrUnits_Manager:
    def __init__(self) -> None:
        self.all_units = []

    def Append_Unit(self, unit: OcrUnit):
        unit.LoadTemplateImageFromFile()
        self.all_units.append(unit)

    def Get_All_TargetAreas(self) -> list[OcrUnit]:
        return self.all_units
    
    def Get_Single_TargetArea(self, name) -> OcrUnit:
        return self.all_units[0]
    

class WindowOcr:
    
    def __init__(self, camera_id, config:OcrUnits_Manager) -> None:
        if (camera_id == 99):
            # capture from screen
            pass
        else:
            self.__cap = cv2.VideoCapture(camera_id)

        self.__config = config
        self.__camer_id = camera_id

    def SpinOnce(self):
        if self.__camer_id == 99:
            area = {'left': 0, 'top': 0, 'width': 1366, 'height': 768}
            with mss() as sct:
                screenShot = sct.grab(area)
                img = Image.frombytes('RGB', (screenShot.width, screenShot.height), screenShot.rgb)
                # img = Image.frombytes('P', (screenShot.width, screenShot.height), screenShot.rgb)

                img2 = np.array(img)
                frame = cv2.cvtColor(img2, cv2.COLOR_BGR2RGB)
                imshow = False
                if imshow:
                    cv2.imshow('origin', np.array(frame))
                    cv2.waitKey(1)
                area = self.__config.Get_Single_TargetArea('youtube')
                # area_image = self.__config.
                cv2.imshow(area.name, frame)
                cv2.waitKey(1)
        else:
            ret, frame = self.__cap.read()
            if ret:
                cv2.imshow('origin', frame)
                cv2.waitKey(1)
                area = self.__config.Get_Single_TargetArea('test')
                area_image = frame
                cv2.imshow(area.name, area_image)

