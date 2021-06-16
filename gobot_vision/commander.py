from vision.grid_finder import GridFinder
from gobot_vision.commander_vision import config_2_aruco_marks


class Commander(GridFinder):
    def __init__(self):
        # self.__mark_ids = [125,126]  # [top, bottom]
        #self.__CELLS = 5
        #self.__LOWEST_SCALE = 0.7
        GridFinder.__init__(self, config_2_aruco_marks)

    def get_command_from_image(self, image):
        '''
        return -1, if found nothing
        return 1..5 if found avaliable cell in black.
        '''
        centers = self.find_corners(image)
        if len(centers)==2:
            
            print('centers',centers)
            top_x, top_y = centers[0]
            bottom_x, bottom_y = centers[1]
            delta_x = (top_x - bottom_x) / 6
            delta_y = (top_y - bottom_y) / 6

            if False:
                # publish a debug image
                color_red = (0,0,255)
                pen_thickness = 3
                cv2.line(image,(top_x,top_y),(bottom_x,bottom_y),color_red, pen_thickness)
                g_mqtt.publish_cv_image('gobot/test/image',image)

            # Get average color
            sum_color = 0
            for index in range(1, self.__CELLS + 1,1):
                # get x,y position of indicator
                x = int(index * delta_x + bottom_x) 
                y = int(index * delta_y + bottom_y) 
                [b,g,r] = image[y, x]
                sum_color += b
                sum_color += g
                sum_color += r

                #print('index=', index,'(x,y)=(',x,y, ')color=', b,g,r)
                #print('cell_position',index, x, y)
            avg_color = sum_color / 5
            
            # Get which cell is black
            for index in range(1, self.__CELLS + 1, 1):
                x = int(index * delta_x + bottom_x)
                y = int(index * delta_y + bottom_y)
                [b,g,r] = image[y,x]
                cell_color = b + g + r
                if cell_color < avg_color * self.__LOWEST_SCALE:
                    return index
        return -1
 
