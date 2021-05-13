import cv2
import numpy as np
from math import sin, cos
from picamera.array import PiRGBArray
from picamera import PiCamera

from mqtt_helper import g_mqtt


class SingleEye():
    '''
        take picture from Pi camera, 
        Find aruco corners
        do perspective transforming.
    '''

    def __init__(self):
        self.__camera = PiCamera()

    def take_picture(self):
        rawCapture = PiRGBArray(self.__camera)
        # grab an image from the camera
        self.__camera.capture(rawCapture, format="bgr")
        image = rawCapture.array
        return image

    def find_corners(self, image, marker_ids):
        '''
        marker_ids is a list of [top_right, bottom_right, bottome_left, top_left]
        '''
        arucoDict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_100)
        arucoParams = cv2.aruco.DetectorParameters_create()
        corners, ids, rejected = cv2.aruco.detectMarkers(image, arucoDict, parameters=arucoParams)
        result = []
        print('double check',marker_ids)
        # verify *at least* one ArUco marker was detected
        if len(corners) > 0:
            # flatten the ArUco IDs list
            ids = ids.flatten()
            for target_id in marker_ids:
                print('----------------Searching.... ', target_id)
                # loop over the detected ArUCo corners
                for (markerCorner, markerID) in zip(corners, ids):
                    if target_id == markerID:
                        print('got matched id', target_id)
                        # extract the marker corners (which are always returned in
                        # top-left, top-right, bottom-right, and bottom-left order)
                        corners2 = markerCorner.reshape((4, 2))
                        topLeft, topRight, bottomRight, bottomLeft = corners2
                        # convert each of the (x, y)-coordinate pairs to integers
                        topRight = (int(topRight[0]), int(topRight[1]))
                        bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
                        bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
                        topLeft = (int(topLeft[0]), int(topLeft[1]))    

                        # compute and draw the center (x, y)-coordinates of the ArUco marker
                        cX = int((topLeft[0] + bottomRight[0]) / 2.0)
                        cY = int((topLeft[1] + bottomRight[1]) / 2.0)

                        print('markid=', markerID, 'center=', (cX, cY),topLeft, bottomRight, bottomLeft, topLeft)
                        result.append ([cX,cY])

                        # print("[INFO] ArUco marker ID: {}".format(markerID))

                        if True:
                            # draw the bounding box of the ArUCo detection
                            cv2.line(image, topLeft, topRight, (0, 255, 0), 2)
                            cv2.line(image, topRight, bottomRight, (0, 255, 0), 2)
                            cv2.line(image, bottomRight, bottomLeft, (0, 255, 0), 2)
                            cv2.line(image, bottomLeft, topLeft, (0, 255, 0), 2)

                            cv2.circle(image, (cX, cY), 4, (0, 0, 255), -1)
                            # draw the ArUco marker ID on the image
                            cv2.putText(image, str(markerID),
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
                            g_mqtt.publish_cv_image('gobot_stonehouse/eye/marker', image)
        return result

    def get_perspective_view(self, img, pts):
        # specify desired output size 
        width = 350
        height = 350

        # specify conjugate x,y coordinates (not y,x)
        # input = np.float32([[62,71], [418,59], [442,443], [29,438]])
        input = np.float32(pts)
        output = np.float32([[0,0], [width-1,0], [width-1,height-1], [0,height-1]])

        # compute perspective matrix
        matrix = cv2.getPerspectiveTransform(input,output)

        print(matrix.shape)
        print(matrix)

        # do perspective transformation setting area outside input to black
        imgOutput = cv2.warpPerspective(img, matrix, (width,height), cv2.INTER_LINEAR, borderMode=cv2.BORDER_CONSTANT, borderValue=(0,0,0))
        print(imgOutput.shape)

        # save the warped output
        return imgOutput

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
