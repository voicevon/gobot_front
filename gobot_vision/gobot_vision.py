

import sys
sys.path.append('D:\\XumingSource\\gobot_front')  # For runing in VsCode on Windows-10 

# from gogame.chessboard import ChessboardLayout
from config.config import Config as app_config
from gobot_vision.commander import Commander
from gobot_vision.commander_vision import CommanderVision
from gobot_vision.chessboard_scanner import ChessboardScanner, config_4_aruco_marks as chessboard_config
from gobot_vision.warehouse_vision import WarehouseVision
from vision.grid_finder import GridFinder

# from von.terminal_font import TerminalFont
import logging
from Pylib.image_logger import ImageLogger


import cv2
from Pylib.message_logger import MessageLogger


from collections import namedtuple
Position = namedtuple("Position",['X','Y'])


class ArucoMarkAdvanced():
    def __init__(self, id:int, corners) -> None:
        self.mark_id = id
        self.corners = corners
        self.GetCenter_xy()

    def GetCenter_xy(self):
        corners2 = self.corners.reshape((4, 2))
        topLeft, topRight, bottomRight, bottomLeft = corners2
        self.corner_topRight = Position(int(topRight[0]), int(topRight[1]))
        self.corner_bottomRight = Position(int(bottomRight[0]), int(bottomRight[1]))
        self.corner_bottomLeft = Position(int(bottomLeft[0]), int(bottomLeft[1]))
        self.corner_topLeft = Position(int(topLeft[0]), int(topLeft[1]))     

        center_x = (self.corner_topLeft.X + self.corner_bottomRight.X) / 2
        center_y = (self.corner_topLeft.Y + self.corner_bottomRight.Y) / 2
        self.center = Position(center_x, center_y)

    def PrintOut(self):
        print('--------------------------------------------')
        print('Aruco Mark ID = ', self.mark_id)
        print('Cebter ',self.center) 
        print(self.corner_topLeft, self.corner_topRight, self.corner_bottomRight,self.corner_bottomLeft,)


class ArucoFinder():
    def __init__(self) -> None:
        self.all_marks = []
        self.predefined_marks = [21,49,48,15,13,34]

    
    def ScanMarks(self, origin_image, print_report=False) -> list:
        '''
        marker_ids is a list of [top_right, bottom_right, bottome_left, top_left]
        This encoding can not find any id which is greater than 250.
        So the maximum id can be used is 249. saying the range is 0 to 249
        '''
        arucoDict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_250)  # pip3 install opencv-contrib-python
        arucoParams = cv2.aruco.DetectorParameters_create()
        corners, ids, rejected = cv2.aruco.detectMarkers(origin_image, arucoDict, parameters=arucoParams)
        '''
        # corners: A list containing the (x, y)-coordinates of our detected ArUco markers
        # ids: The ArUco IDs of the detected markers
        # rejected: A list of potential markers that were found but ultimately rejected due to the inner code of the marker being unable to be parsed (visualizing the rejected markers is often useful for debugging purposes)
        '''
        if ids is None:
            return
        
        if print_report:
            MessageLogger.Output('GridFinder.find_corners()  found ids', ids)
            known_marks = self.predefined_marks.copy()
            for id in ids:
                if id in known_marks:
                    known_marks.remove(id)
                else:
                    print("[Error]  ScanMarks() got unknown id {id} " )

            print('Known id can not detected:  ', end=' ')
            for mark in known_marks:
                print(mark, end = '  ')
            print()

        ids = ids.flatten()
        self.all_marks=[]
        for (markerCorners, markerID) in zip(corners, ids):
            new_marker = ArucoMarkAdvanced(markerID, markerCorners)
            # new_marker.PrintOut()
            self.all_marks.append(new_marker)
        return

        # return ids


        result = []
        # verify *at least* one ArUco marker was detected
        if len(corners) >= len(self.__mark_ids):
            # if Config.publish_mqtt:
            if True:
                debug_image = origin_image.copy()
            # get rid of useless corners

            # flatten the ArUco IDs list
            ids = ids.flatten()
            current_corner_index = 1
            for target_id in self.__mark_ids:
                #print('----------------Searching.... ', target_id)
                # loop over the detected ArUCo corners
                for (markerCorner, markerID) in zip(corners, ids):
                    if target_id == markerID:
                        #print('got matched id', target_id)
                        # extract the marker corners (which are always returned in order of:
                        #   [top-left, top-right, bottom-right, and bottom-left]
                        corners2 = markerCorner.reshape((4, 2))
                        topLeft, topRight, bottomRight, bottomLeft = corners2
                        # convert each of the (x, y)-coordinate pairs to integers
                        topRight = (int(topRight[0]), int(topRight[1]))
                        bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
                        bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
                        topLeft = (int(topLeft[0]), int(topLeft[1]))    
                        if len(self.__mark_ids) == 4:
                            if current_corner_index == 1:
                                # top left corner of the plate, we take bottom right point of the marker.
                                result.append(bottomLeft)
                            if current_corner_index == 2:
                                # bottom right corner of the plate, we take top right point of the marker.
                                result.append(bottomRight)
                            if current_corner_index == 3:
                                # bottom left corner of the plate, we take top left point of the marker
                                result.append(topRight)
                            if current_corner_index == 4:
                                # top left corner of the plate, we take top bottom point of the marker
                                result.append(topLeft)

                        if len(self.__mark_ids) == 2:
                            # keep the result in anticlockwise.
                            if current_corner_index == 1:
                                # right mark of the plate, we append two left points of the marker
                                result.append(topLeft)
                                result.append(bottomLeft)
                            if current_corner_index == 2:
                                # left mark of the plate, we append two right points of the marker
                                result.append(bottomRight)
                                result.append(topRight)


                        # print("[INFO] ArUco marker ID: {}".format(markerID))
                        # current_corner_index += 1

                        # if Config.publish_mqtt:
                        if True:
                            # compute and draw the center (x, y) - coordinates of the ArUco marker
                            cX = int((topLeft[0] + bottomRight[0]) / 2.0)
                            cY = int((topLeft[1] + bottomRight[1]) / 2.0)
                            # print('markid=', markerID, 'center=', (cX, cY),topLeft, bottomRight, bottomLeft, topLeft)
                            # draw the bounding box of the ArUCo detection
                            if current_corner_index == 1:
                                color = (0,0,255)
                                pen_thickness = 5
                            else:
                                color = (0,255,0)
                                pen_thickness = 2
                            cv2.line(debug_image, topLeft, topRight, color, pen_thickness)
                            cv2.line(debug_image, topRight, bottomRight, color, pen_thickness)
                            cv2.line(debug_image, bottomRight, bottomLeft, color, pen_thickness)
                            cv2.line(debug_image, bottomLeft, topLeft, color, pen_thickness)

                            cv2.circle(debug_image, (cX, cY), 4, (0, 0, 255), -1)
                            # draw the ArUco marker ID on the image
                            cv2.putText(debug_image, str(markerID),
                                        (topLeft[0], topLeft[1] - 15), cv2.FONT_HERSHEY_SIMPLEX,
                                        8, (0, 255, 0), 2)

                            # img_marker = self.draw_axis(image,0,0,0)


                            # show the output image
                    
                            # rvec, tvec = cv2.aruco.estimatePoseSingleMarkers(corners, 0.05, mtx, dist) #Estimate pose of each marker and return the values rvet and tvec---different from camera coeficcients  
                            # (rvec-tvec).any() # get rid of that nasty numpy value array error  
                            
                            # cv2.aruco.drawAxis(image, mtx, dist, rvec, tvec, 0.1) #Draw Axis  
                            # cv2.aruco.drawAxis(image, )
                            # cv2.aruco.drawDetectedMarkers(image, corners) #Draw A square around the markers  

                            # image = cv2.aruco.drawMarker(cv2.aruco.DICT_4X4_1000,)
                            # image = self.draw_axis_2(image, corners)
                        current_corner_index += 1
            ImageLogger.Output('grid/aruco', debug_image)
            # if Config.publish_mqtt:
            #     g_mqtt.publish_cv_image('gobot/image/grid/aruco', debug_image)
        return result


class GobotVision():

    def __init__(self):
        '''
        1. Overview of origin_image.
        2. Process:
           * Find all aruco marks, and there position
           * Get perspectived veiws of command_image, board_image, house_vendor_image
           * Scan the segmented images, To get command, board_layout, house_vender_stone.
        '''

        self.__chessboard_scanner = ChessboardScanner()
        config = self.__chessboard_scanner.get_4_aruco_marks_config()
        self.__chessboard_grid_finder = GridFinder(config)

        self.__commander_solution = 2
        if self.__commander_solution == 1:
            # solution A
            # unified to chessboard_vision, might be slower
            self.__commander_vision = CommanderVision()
            config = self.__commander_vision.create_2_aruco_marks_config()
            self.__commander_grid_finder = GridFinder(config)

        if self.__commander_solution == 2:
            # solution B
            # Simpler solution, faster, might be less stable
            self.__commander = Commander()

        self.__publish_image = False
        logging.warn('Init vision is done......')

    def init_chessboard_layout(self):
        self.__chessboard_scanner.create_blank_layout()
        
    def get_stable_level (self, layout_history):
        stable_level = 0
        if layout_history[0][0][0] == layout_history[1][0][0]:
            stable_level += 1
        return stable_level

    def get_commander_plate_image(self, origin_image):
        '''
        Serve for Solution A, Not completed.
        '''
        commander_grid_image = self.__commander_grid_finder.detect_grid_from_aruco_corners()
        return commander_grid_image

    def get_command_index(self, origin_image):
        return self.__commander.get_command_from_image(origin_image)

    def get_chessboard_layout(self, origin_image):
        '''
        * Top level of getting layout.
        return 
        * layout, stable_depth. 
        * if stable_depth <= 0 , is saying can not get board image.
        '''
        perspective_image = self.__chessboard_grid_finder.detect_grid_from_aruco_corners(origin_image)
        if perspective_image is None:
            return None, -1
        
        # We got 4 corners. So we can do two (maybe 3 )things here
        # 1. Get pespectived image of chessboard
        # 2. Get house vender position(and house vender perspectived image)
        
        x0 = chessboard_config.crop_x0
        x1 = x0 + chessboard_config.crop_width
        y0 = chessboard_config.crop_y0
        y1 = y0 + chessboard_config.crop_height
        board_image = perspective_image[y0:y1, x0:x1]
        self.house_vendor_image = perspective_image[1:2,  3:4]

        if app_config.publish_image_board.value:
            ImageLogger.Output('gobot_image_board', perspective_image)
        if board_image is None:
            print('GobotVision.get_chessboard_layout()  Can NOT detect chessboard grid from origin_image')
            return None, 0

        layout, stable_depth = self.__chessboard_scanner.start_scan(board_image,3,True)
            #::wqlayout.print_out() 
        #print ('Stable Depth of the layout ', stable_depth)
        return (layout, stable_depth)

    def get_warehouse_plate(self,origin_image):
        config = WarehouseVision().create_finder_config()
        house_plate_finder = GridFinder(config)
        perspective_image = house_plate_finder.detect_grid_from_aruco_corners(origin_image)

        ImageLogger.Output("gobot_vision_perspective", perspective_image)

    def get_warehouse_stone_position(self, origin_image):
        '''
        Top level interface.
        '''
        return 1

    def get_commander_layout_for_solution_A_only(self, image, min_stable_depth=3, max_trying =5):
        '''
        Serve for Solution A, Not completed.
        '''
        history = []
        layout = self.__commander_vision.get_layout_from_image(image)
        history.append(layout)
        stable_count = self.get_stable_level(history)
        if stable_count >= min_stable_depth:
            return True,layout
        
        return False, -1

    def get_command_index_solution_A(self, origin_image):
        '''
        Deprecated solution.
        '''
        image = self.__commander_grid_finder.detect_grid_from_aruco_corners(origin_image)
        ret, layout = self.get_commander_layout_for_solution_A_only(image)
        if ret:
            for i in range(0,5,1):
                if layout[i] == BLACK_STONE:
                    return i
        return -1


if __name__ == '__main__':
    my_vision = GobotVision()
    history = []
    for i in range(0,6,1):
        layout = ChessboardScanner().create_blank_layout()
        history.append(layout)
    is_same = my_vision.is_same_layout(history)
    print(is_same)
    
