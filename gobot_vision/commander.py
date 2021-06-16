import cv2
from vision.grid_finder import GridFinder
from gobot_vision.commander_vision import config_2_aruco_marks
import sys
sys.path.append('/home/pi/pylib')
from mqtt_helper import g_mqtt

class Commander(GridFinder):
    def __init__(self):
        # self.__mark_ids = [125,126]  # [top, bottom]
        self.__CELLS = 5
        self.__LOWEST_SCALE = 0.7
        GridFinder.__init__(self, config_2_aruco_marks)

    def get_command_from_image(self, image):
        '''
        return -1, if found nothing
        return 1..5 if found avaliable cell in black.
        '''
        corners = self.find_corners(image)
        #print ('len(centers) =', len(centers))
        #print(centers)
        if len(corners) == 4: # Sucessful detection will return 4 corners
            print('corners',corners)

            x1, y1 = corners[0]
            x2, y2 = corners[1]
            x3, y3 = corners[2]
            x4, y4 = corners[3]
            cx1 = int((x1 + x2) / 2)
            cy1 = int((y1 + y2) / 2)
            cx2 = int((x3 + x4) / 2)
            cy2 = int((y3 + y4) / 2)
            
            delta_x = (cx2 - cx1) / 5 
            delta_y = (cy2 - cy1) / 5

            if True:
                # publish a debug image
                color_red = (0,0,255)
                pen_thickness = 3
                cv2.line(image,(cx1, cy1),(cx2, cy2),color_red, pen_thickness)
                g_mqtt.publish_cv_image('gobot/test/image',image)

            # Get average color
            sum_color = 0
            for index in range(0, self.__CELLS + 1,1):
                # get x,y position of indicator
                x = int(index * delta_x + cx1 + delta_x / 2)
                y = int(index * delta_y + cy1 + delta_y / 2) 
                [b,g,r] = image[y, x]
                sum_color += b
                sum_color += g
                sum_color += r

                #print('index=', index,'(x,y)=(',x,y, ')color=', b,g,r)
                #print('cell_position',index, x, y)
            avg_color = sum_color / 5
            
            # Get which cell is black
            for index in range(1, self.__CELLS + 1, 1):
                x = int(index * delta_x + cx1 + delta_x / 2) 
                y = int(index * delta_y + cy1 + delta_y / 2)
                [b,g,r] = image[y,x]
                cell_color = b + g + r
                if cell_color < avg_color * self.__LOWEST_SCALE:
                    return index
        return -1
 
