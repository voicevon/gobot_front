from von.mqtt.remote_var_mqtt import RemoteVar_mqtt, g_mqtt
from von.logger import Logger
import cv2
import pytesseract
from PIL import Image #pip install pillow


class ScreenImageDivider:

    def __init__(self, areas) -> None:
        '''
        areas list in json
        '''
        self.__areas = areas

            
    def DivideToSmallImages(self, window_image):
        '''
        return a list of small images, 
        Notice:
        in OpenCV-format, not PIL-format
        '''
        left, top = 0, 0
        small_images = []

        for area in self.__areas:
            x1 = area['x1']
            y1 = area['y1']
            x2 = area['x2']
            y2 = area['y2']
            cropped_image = window_image[y1:y2, x1:x2]
            small_images.append(cropped_image)

            do_more = False
            if do_more:
                color_converted = cv2.cvtColor(cropped_image, cv2.COLOR_BGR2RGB)
                pil_image =  Image.fromarray(color_converted)
                string = pytesseract.image_to_string(pil_image)
                Logger.Print("pytesseract string", string)

            debug = True
            if debug:
                if len(small_images) >= 2:
                    return small_images
        return small_images



