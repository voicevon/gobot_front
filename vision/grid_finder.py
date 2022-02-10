# These two references are really not good !
# Like lower class is reference higher class !



#                           
#
#                     The number and position of arucodes
#            
#                           34 ---------- 21
#                           |             |
#                           |             |
#                           |  Chessboard |
#                           |             |
#                           13 --------- 15
#                             \          /
#                              \ House  /
#                               --------
#                             



# TODO: Reconstruct 
from gobot_vision.commander_vision import config_2_aruco_marks
from gobot_vision.chessboard_vision import config_4_aruco_marks


import cv2
import numpy as np
from math import sin, cos

import sys
sys.path.append ("/home/pi/pylib")
from mqtt_helper import g_mqtt
from config.config import config


class GridFinder():
    '''
    '''
    def __init__(self, aruco_config):
        '''   
        Supported board_types:
            Warehouse: Four aruco marks
            Commander: Two aruco marks.
            Go_game_board_19x19: Normally Two aruco marks ?
                                 Calibrate mode, Four marks ?
        '''
        self.__aruco_config = aruco_config

        print(aruco_config.name)
        if aruco_config.name=='CHESSBOARD_ARUCO_CONFIG':
            #print('44444')
            aruco_ids = [aruco_config.top_left_id, aruco_config.top_right_id, aruco_config.bottom_right_id, aruco_config.bottom_left_id]
            # self.__perspected_width = aruco_config.perspected_width
            # self.__perspected_height = aruco_config.perspected_height
        elif aruco_config.name == 'COMMANDER_ARUCO_CONFIG':    #print('222222222')
            aruco_ids = [aruco_config.right_id, aruco_config.left_id]
        
        elif aruco_config.name == 'WAREHOUSE_ARUCO_CONFIG':
            aruco_ids = [aruco_config.top_left_id, aruco_config.top_right_id, aruco_config.bottom_right_id, aruco_config.bottom_left_id]
            # self.__perspected_width = aruco_config.perspected_width
            # self.__perspected_height = aruco_config.perspected_height

        else:
            print('Unknown config of aruco_marks', aruco_config.name)
        self.__mark_ids =  aruco_ids
        print('GridFinder init is OK')

        # self.__area_width  = aruco_config.width
        # self.__area_height = aruco_config.height


    def find_corners(self, image) -> list:
        '''
        marker_ids is a list of [top_right, bottom_right, bottome_left, top_left]
        This encoding can not find any id which is greater than 250.
        So the maximum id can be used is 249. saying the range is 0 to 249
        '''
        arucoDict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_250)
        arucoParams = cv2.aruco.DetectorParameters_create()
        corners, ids, rejected = cv2.aruco.detectMarkers(image, arucoDict, parameters=arucoParams)
        # result = []
        print('GridFinder.find_corners()  double check target ids ',self.__mark_ids)
        print('GridFinder.find_corners()  found ids ', ids)
        result = []
        # verify *at least* one ArUco marker was detected
        if len(corners) >= len(self.__mark_ids):
            if config.publish_mqtt:
                debug_image = image.copy()
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

                        if config.publish_mqtt:
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
            if config.publish_mqtt:
                g_mqtt.publish_cv_image('gobot/image/grid/aruco', debug_image)
        return result


    def get_perspective_view(self, img, pts):
        # specify desired output size 

        # specify conjugate x,y coordinates (not y,x)
        input = np.float32(pts)
        x1 = 0 + self.__aruco_config.left_extended
        y1 = 0 + self.__aruco_config.top_extended
        x2 = x1 + self.__aruco_config.perspective_width 
        y2 = y1
        x3 = x2
        y3 = y1 +self.__aruco_config.perspective_height
        x4 = x1
        y4 = y3
        width = x3 + self.__aruco_config.right_extended
        height = y3 + self.__aruco_config.bottom_extended
    

        output = np.float32([[x1,y1],[x2,y2],[x3,y3],[x4,y4]])
        # output = np.float32([[0 ,0] [width-1 ,0], [width-1,height-1], [0,height-1]])
        #output = np.float32([[0,0],[width,0],[width/4*3,height/2],[width/4,height/2]])
        # compute perspective matrix
        matrix = cv2.getPerspectiveTransform(input,output)

        # print(matrix.shape)
        # print(matrix)

        # do perspective transformation setting area outside input to black
        imgOutput = cv2.warpPerspective(img, matrix, (width,height), cv2.INTER_LINEAR, borderMode=cv2.BORDER_CONSTANT, borderValue=(0,0,0))
        # print(imgOutput.shape)

        # save the warped output
        return imgOutput

    def detect_grid_from_aruco_corners(self, origin_image):
        # Get corners position from detecting aruco marks
        corners = self.find_corners(origin_image)
        # print(corners)
        if corners != None:
            if len(corners) >= len(self.__mark_ids):
                # Get perspectived image
                perspect_img = self.get_perspective_view(origin_image,corners)
                return perspect_img
        return None

    
    def get_chessboard_image(self,img):
        # https://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
        # https://www.geeksforgeeks.org/perspective-transformation-python-opencv/

        # detect edges using gray, then Canny
        img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        canny = cv2.Canny(img_gray, 120,200)
        # retrieve contours by findCountours
        contours, hierarchy = cv2.findContours(canny, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        # img_contour = cv2.drawContours(img, contours, -1, (0,255,75), 1)
        # cv2.imshow('contours',img_contour)
        for con in contours:
            # rec = cv2.boundingRect(con)
            area = cv2.contourArea(con)
            if area > 17000:
                perspectived = self.get_perspective_from_contour(img, con)
                if perspectived is not None:
                    whole_board_image = perspectived[0:self.__CROP_HEIGHT, 0:self.__CROP_WIDTH]
                    cv2.imshow('whole_board', whole_board_image)
                    cv2.waitKey(1)
                    self.__show_detection_line(whole_board_image)
                    return whole_board_image



    def draw_axis(self,img, yaw, pitch, roll, tdx=None, tdy=None, size = 100):

        pitch = pitch * np.pi / 180
        yaw = -(yaw * np.pi / 180)
        roll = roll * np.pi / 180

        if tdx != None and tdy != None:
            tdx = tdx
            tdy = tdy
        else:
            height, width = img.shape[:2]
            tdx = width / 2
            tdy = height / 2

        # X-Axis pointing to right. drawn in red
        x1 = size * (cos(yaw) * cos(roll)) + tdx
        y1 = size * (cos(pitch) * sin(roll) + cos(roll) * sin(pitch) * sin(yaw)) + tdy

        # Y-Axis | drawn in green
        #        v
        x2 = size * (-cos(yaw) * sin(roll)) + tdx
        y2 = size * (cos(pitch) * cos(roll) - sin(pitch) * sin(yaw) * sin(roll)) + tdy

        # Z-Axis (out of the screen) drawn in blue
        x3 = size * (sin(yaw)) + tdx
        y3 = size * (-cos(yaw) * sin(pitch)) + tdy

        cv2.line(img, (int(tdx), int(tdy)), (int(x1),int(y1)),(0,0,255),3)
        cv2.line(img, (int(tdx), int(tdy)), (int(x2),int(y2)),(0,255,0),3)
        cv2.line(img, (int(tdx), int(tdy)), (int(x3),int(y3)),(255,0,0),2)

        return img 

    def draw_axis_2(self, frame, corners):
        dist=np.array(([[-0.58650416 , 0.59103816, -0.00443272 , 0.00357844 ,-0.27203275]]))
        newcameramtx=np.array([[189.076828   ,  0.    ,     361.20126638]
                            ,[  0 ,2.01627296e+04 ,4.52759577e+02]
                            ,[0, 0, 1]])
        mtx=np.array([[398.12724231  , 0.      ,   304.35638757],
                        [  0.       ,  345.38259888, 282.49861858],
                        [  0.,           0.,           1.        ]])

        rvec, tvec, _ = cv2.aruco.estimatePoseSingleMarkers(corners, 0.05, mtx, dist)
        # 估计每个标记的姿态并返回值rvet和tvec ---不同
        # from camera coeficcients
        (rvec-tvec).any() # get rid of that nasty numpy value array error
        i = 0
        cv2.aruco.drawAxis(frame, mtx, dist, rvec[i, :, :], tvec[i, :, :], 0.03)
        cv2.aruco.drawDetectedMarkers(frame, corners)
        return frame


