
from math import sin, cos
from picamera.array import PiRGBArray
from picamera import PiCamera

import numpy as np
import cv2 as cv
import glob



class MonoEye():
    '''
    take picture from Pi camera, 
    TODO: Calibration
    https://medium.com/vacatronics/3-ways-to-calibrate-your-camera-using-opencv-and-python-395528a51615
    '''

    def __init__(self, coefficients_file):
        self.__camera = PiCamera(resolution=(1920,1088))
        #self.__rawCapture = PiRGBArray(self.__camera)
        self.__coefficients_file = coefficients_file
        # mtx, dist = self.load_coefficients('calibration_chessboard.yml')
        mtx, dist = self.load_coefficients(coefficients_file)
        self.__mtx = mtx
        self.__dist = dist

    def take_picture(self, do_undistort=True):
        rawCapture = PiRGBArray(self.__camera)
        # grab an image from the camera
        self.__camera.capture(rawCapture, format="bgr")
        image = rawCapture.array
        if do_undistort:
            image = cv2.undistort(image, self.__mtx, self.__dist, None, None)
        return image

    def take_batch_picture_for_calibration(self):
        while True:
            self.take_picture(do_undistort=False)
            key = cv2.waitkey(1)
            if key == 'ESC':
                return

    def calibrate_chessboard(self, dir_path, image_format, square_size, width, height):
        '''Calibrate a camera using chessboard images.'''
        # termination criteria
        criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
        
        # prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(8,6,0)
        objp = np.zeros((height*width, 3), np.float32)
        objp[:, :2] = np.mgrid[0:width, 0:height].T.reshape(-1, 2)

        objp = objp * square_size

        # Arrays to store object points and image points from all the images.
        objpoints = []  # 3d point in real world space
        imgpoints = []  # 2d points in image plane.

        images = pathlib.Path(dir_path).glob(f'*.{image_format}')
        # Iterate through all images
        for fname in images:
            img = cv2.imread(str(fname))
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

            # Find the chess board corners
            ret, corners = cv2.findChessboardCorners(gray, (width, height), None)

            # If found, add object points, image points (after refining them)
            if ret:
                objpoints.append(objp)

                corners2 = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
                imgpoints.append(corners2)

        # Calibrate camera
        ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

        return [ret, mtx, dist, rvecs, tvecs]

    def save_coefficients(self, mtx, dist, path):
        '''Save the camera matrix and the distortion coefficients to given path/file.'''
        cv_file = cv2.FileStorage(path, cv2.FILE_STORAGE_WRITE)
        cv_file.write('K', mtx)
        cv_file.write('D', dist)
        # note you *release* you don't close() a FileStorage object
        cv_file.release()

    def load_coefficients(self, path):
        '''Loads camera matrix and distortion coefficients.'''
        # FILE_STORAGE_READ
        cv_file = cv2.FileStorage(path, cv2.FILE_STORAGE_READ)

        # note we also have to specify the type to retrieve other wise we only get a
        # FileNode object back instead of a matrix
        camera_matrix = cv_file.getNode('K').mat()
        dist_matrix = cv_file.getNode('D').mat()

        cv_file.release()
        return [camera_matrix, dist_matrix]

    def recalibrate_and_save_coefficients(self):
        # Parameters
        IMAGES_DIR = 'chessboard_samples'
        IMAGES_FORMAT = '.jpg'
        SQUARE_SIZE = 1.6
        WIDTH = 6
        HEIGHT = 9

        # Calibrate 
        ret, mtx, dist, rvecs, tvecs = self.calibrate_chessboard(
            IMAGES_DIR, 
            IMAGES_FORMAT, 
            SQUARE_SIZE, 
            WIDTH, 
            HEIGHT
        )
        # Save coefficients into a file
        self.save_coefficients(mtx, dist, self.__coefficients_file)


class SteroEye():
    def __init__(self):
        self.left = MonoEye('left_coefficients.yml')
        self.right = MonoEye('right_coefficients.yml')



if __name__ == '__main__':
    my_eye = MonoEye('2021-0611.yml')
    if True:
        my_eye.take_batch_picture_for_calibration()
    my_eye.recalibrate_and_save_coefficients()
    

