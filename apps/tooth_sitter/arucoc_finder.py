import cv2
from collections import namedtuple
from von.logger import Logger

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
    def __init__(self, predefined_marks) -> None:
        self.all_marks = [ArucoMarkAdvanced]
        self.predefined_marks = predefined_marks
    
    def ScanMarks(self, origin_image, print_report=False) -> list:
        '''
        marker_ids is a list of [top_right, bottom_right, bottome_left, top_left]
        This encoding can not find any id which is greater than 250.
        So the maximum id can be used is 249. saying the range is 0 to 249
        '''
        arucoDict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_250)
        arucoParams =  cv2.aruco.DetectorParameters()
        corners, ids, rejected = cv2.aruco.detectMarkers(origin_image, arucoDict, parameters=arucoParams)
        '''
        # corners: A list containing the (x, y)-coordinates of our detected ArUco markers
        # ids: The ArUco IDs of the detected markers
        # rejected: A list of potential markers that were found but ultimately rejected due to the inner code of the marker being unable to be parsed (visualizing the rejected markers is often useful for debugging purposes)
        '''
        if ids is None:
            return []
        
        if print_report:
            Logger.Print('GridFinder.find_corners()  found ids', ids)
            known_marks = self.predefined_marks.copy()
            for id in ids:
                if id in known_marks:
                    known_marks.remove(id)
                else:
                    print("[Error]  ScanMarks() got unknown id {id} " )

            if len(known_marks) >0:
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
        return self.all_marks

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

    def FindMarkById(self, id:int)->ArucoMarkAdvanced:
        for mark in self.all_marks:
            if mark.mark_id == id:
                return mark
        return None


    def GetPoints_For_PespectiveInput(self, mark_ids)->list:
        '''
        The return is for input parameters of cv2.getPerspectiveTransform(input,output) \n
        mark_ids = [top-left, top-right, bottom-right, and bottom-left]
        '''
        # extract the marker corners (which are always returned in order of:
        #   
        points = []
        for mm in mark_ids:
            xx = self.FindMarkById(mm)
            if xx is None:
                return []
            point = (int(xx.center.X), int(xx.center.Y))
            # print(point)
            points.append(point)
        return points