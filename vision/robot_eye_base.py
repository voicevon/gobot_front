
from math import sin, cos


import numpy as np
import cv2  # pip3 install opencv-python  #sudo apt-get install python3-pip 
            # for windows-10, pip3 install opencv-contrib-python
import glob  #import keyboard   #pip3 install keyboard, Doesn't work on Pi zero
import pathlib
import logging
from abc import ABC, abstractmethod, abstractproperty


class MonoEyeBase(ABC):
    '''
    take picture from Pi camera, 
    Calibration Tutourial  https://medium.com/vacatronics/3-ways-to-calibrate-your-camera-using-opencv-and-python-395528a51615

    TODO Make an abstract hardware robotEye, So can debug software on any computer.
    '''
    __show_debug_info = True

    @property
    @classmethod
    def show_debug_info(cls) ->bool:
        return cls.__show_debug_info

    @show_debug_info.setter
    def show_debug_info(cls, value):
        cls.__show_debug_info = value




    @abstractmethod
    def take_picture(self, do_undistort=True):
        pass
    
    def take_batch_picture_for_calibration(self):
        file_id = 1
        WIDTH = 6
        HEIGHT = 9
        # termination criteria
        criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
        # prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
        objp = np.zeros((WIDTH * HEIGHT, 3), np.float32)
        objp[:,:2] = np.mgrid[0:WIDTH,0:HEIGHT].T.reshape(-1,2)
        # Arrays to store object points and image points from all the images.
        objpoints = [] # 3d point in real world space
        imgpoints = [] # 2d points in image plane.
        print('Start detecting chessboard for camera calibration...')
        while True:
            image = self.take_picture(do_undistort=False)
            gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

            # Find the chess board corners
            #print('before invoking, This might cause a minite watting if the chessboard is out of camera view.')
            # resize the image can be faster
            small_frame = cv2.resize(gray,(0,0), fx=0.3, fy=0.3)
            chessboard_flags = cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_FAST_CHECK + cv2.CALIB_CB_NORMALIZE_IMAGE
            ret, corners = cv2.findChessboardCorners(small_frame, (WIDTH, HEIGHT), chessboard_flags)
            if ret:
                ret, corners = cv2.findChessboardCorners(gray, (WIDTH,HEIGHT), None)
                #print ('after findChessboardCorners() invoking')
                # If found, add object points, image points (after refining them)
                if ret:
                    objpoints.append(objp) # Certainly, every loop objp is the same, in 3D.
                    corners2 = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
                    imgpoints.append(corners2)
                    # Draw and display the corners
                    cv2.drawChessboardCorners(gray, (WIDTH,HEIGHT), corners2, ret)
                    g_mqtt.publish_cv_image('gobot/test/image', gray)

                    key = input('press "s" to skip, <space> to save image to file, "q" to quit sampling   >>> ')
                    if key ==' ':
                        # space is pressed
                        filename = self.__CALIBRATION_IMAGE_PATH + 'origin_' + str(file_id) + '.jpg'
                        cv2.imwrite(filename, image)
                        print('file is saved as ', filename)
                        file_id += 1
                    if key == 's':
                        pass
                    if key == 'q':
                        return
            else:
                g_mqtt.publish_cv_image('gobot/test/image',image)
                print('Can not detect chessboard , trying another time')

    def calibrate_chessboard(self):
        WIDTH = 6
        HEIGHT = 9
        SQUARE_SIZE = 2.5
        image_format = "jpg"
        '''Calibrate a camera using chessboard images.'''
        # termination criteria
        criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
        
        # prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(8,6,0)
        objp = np.zeros((HEIGHT*WIDTH, 3), np.float32)
        objp[:, :2] = np.mgrid[0:WIDTH, 0:HEIGHT].T.reshape(-1, 2)

        objp = objp * SQUARE_SIZE

        # Arrays to store object points and image points from all the images.
        objpoints = []  # 3d point in real world space
        imgpoints = []  # 2d points in image plane.

        images = pathlib.Path(self.__CALIBRATION_IMAGE_PATH).glob(f'*.{image_format}')
        # Iterate through all images
        # print(images)
        for fname in images:
            print('fname', fname)
            img = cv2.imread(str(fname))
            g_mqtt.publish_cv_image('gobot/test/image', img)
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

            # Find the chess board corners
            ret, corners = cv2.findChessboardCorners(gray, (WIDTH, HEIGHT), None)

            # If found, add object points, image points (after refining them)
            if ret:
                print('    Found chessbord')
                objpoints.append(objp)

                corners2 = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
                imgpoints.append(corners2)
            else:
                print('    NOT  found chessboard')

        print('All files are processed')
        # Calibrate camera
        ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

        return [ret, mtx, dist, rvecs, tvecs]

    def load_coefficients(self, path):
        '''Loads camera matrix and distortion coefficients.'''
        # FILE_STORAGE_READ
        print('RobotEye.load_coefficients()  path=',path)
        cv_file = cv2.FileStorage('vision/'+path, cv2.FILE_STORAGE_READ)
        #print ('cv_file', cv_file)

        # note we also have to specify the type to retrieve other wise we only get a
        # FileNode object back instead of a matrix
        camera_matrix = cv_file.getNode('K').mat()
        dist_matrix = cv_file.getNode('D').mat()

        cv_file.release()
        print ('RobotEye ---- Camera calibration data:', camera_matrix, dist_matrix)
        return [camera_matrix, dist_matrix]

    def recalibrate_and_save_coefficients(self):
        '''Save the camera matrix and the distortion coefficients to given path/file.'''
        # Parameters
        IMAGES_FORMAT = '.jpg'
        # Calibrate 
        ret, mtx, dist, rvecs, tvecs = self.calibrate_chessboard()
        # Save coefficients into a file
        #print('aaaaaaaaaa')
        cv_file = cv2.FileStorage(self.__coefficients_file, cv2.FILE_STORAGE_WRITE)
        #print('bbbbbbb')
        cv_file.write('K', mtx)
        cv_file.write('D', dist)
        # note you *release* you don't close() a FileStorage object
        cv_file.release()
        print('Calibration is done. ')

class SteroEye():
    def __init__(self):
        self.left = MonoEyeBase('left_coefficients.yml')
        self.right = MonoEyeBase('right_coefficients.yml')




    

