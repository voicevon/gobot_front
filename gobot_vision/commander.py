from Pylib.image_logger import ImageLogger
import cv2
import logging


import sys
sys.path.append('D:\\XumingSource\\gobot_front')  # For runing in VsCode on Windows-10 
from vision.grid_finder import GridFinder
from gobot_vision.commander_vision import config_2_aruco_marks
from config.config import Config as app_config

class Commander(GridFinder):
    def __init__(self):
        self.__CELLS = 5
        self.__LOWEST_SCALE = 0.7
        GridFinder.__init__(self, config_2_aruco_marks)
        # print("There is no error...................xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,  How does Python work?")

    def get_command_from_image(self, origin_image) -> int:
        '''
        return -1, if found nothing
        return 1..5 if found avaliable cell in black.
        '''
        #if config.publish_image_command_soultion2:
        gray_image = cv2.cvtColor(origin_image,cv2.COLOR_BGR2GRAY)
        if app_config.publish_image_command:
            debug_image = origin_image.copy()
        corners = self.find_corners(gray_image)
        if len(corners) == 4: 
            # Sucessful detection will return 4 corners
            x1, y1 = corners[0]
            x2, y2 = corners[1]
            x3, y3 = corners[2]
            x4, y4 = corners[3]
            # get two center points 
            cx1 = int((x1 + x2) / 2)
            cy1 = int((y1 + y2) / 2)
            cx2 = int((x3 + x4) / 2)
            cy2 = int((y3 + y4) / 2)
            
            delta_x = (cx2 - cx1) / self.__CELLS 
            delta_y = (cy2 - cy1) / self.__CELLS

            if app_config.publish_image_command:
                # publish a debug image. Draw a center line. For debug only.
                # this code will effect image to get correct command_index
                color_red = (0,0,255)
                pen_thickness = 3
                cv2.line(debug_image,(cx1, cy1),(cx2, cy2),color_red, pen_thickness)
                ImageLogger.Output('command',debug_image)

            # Get average color
            sum_brightness = 0
            for index in range(0, self.__CELLS , 1):
                # get x,y position of indicator
                x = int(index * delta_x + cx1 + delta_x / 2)
                y = int(index * delta_y + cy1 + delta_y / 2)
                if app_config.publish_image_command:
                    cv2.circle(debug_image,(x,y),20,(0,255,0),5)
                    ImageLogger.Output('command',debug_image)
                brightness = gray_image[y, x]
                #sum_color += b
                #sum_color += g
                sum_brightness += brightness

                #print('index=', index,'(x,y)=(',x,y, ')color=', b,g,r)
                #print('cell_position',index, x, y)
            avg_brightness = sum_brightness / self.__CELLS
            # logging.info('sum_brightness=%d, avg_brightness=%d',sum_brightness,avg_brightness)
        
            # Get which cell is black
            for index in range(0, self.__CELLS, 1):
                x = int(index * delta_x + cx1 + delta_x / 2) 
                y = int(index * delta_y + cy1 + delta_y / 2)
                cell_brightness = gray_image[y,x]
                #cell_color = b + g + r
                # logging.info('cell[%d] brightness=%d', index, cell_brightness)
                if cell_brightness < avg_brightness * self.__LOWEST_SCALE:
                    return index
        logging.warning('Can NOT find arucos')
        return -1
 
