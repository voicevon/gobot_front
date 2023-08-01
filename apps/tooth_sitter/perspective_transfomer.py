import cv2, numpy as np

class config_4_aruco_marks:
    name='CHESSBOARD_ARUCO_CONFIG'
    top_right_id =  13
    bottom_right_id = 34
    bottom_left_id = 21
    top_left_id = 15

    # perspective_width = 428 -6   #effect scale, greater number cause zoom in. 
    # perspective_height = 428 + 5  # effect scale
    # top_extended = -7
    # right_extended = 5   # effect view range
    # bottom_extended = 5   # effect view range
    # left_extended = 0

    left_extended = 0
    ''' effects view range, greater number causes zoom out, chessboard looks smaller '''   
    right_extended = 0
    ''' * effects view range, greater number causes zoom out, chessboard looks smaller ''' 
    perspective_width = 428 - 32
    ''' Effects scale, greater number causes zoom in. chessboard looks bigger '''  


    perspective_height = 428 + 40
    ''' Effects scale, greater number causes zoom in. chessboard looks bigger'''
    top_extended = 0
    ''' effects view range, greater number causes zoom out, chessboard looks smaller '''   
    bottom_extended = 0   # effect view range
    ''' effects view range, greater number causes zoom out, chessboard looks smaller '''   


    crop_x0 = 0   # effect the left edge
    crop_y0 = 0   # effect the top edge.
    crop_width = 428   # Don't modify this value
    crop_height = 428  # Don't modify this value.


class PerspectiveTransformer():

    def __init__(self) -> None:
        self.__aruco_config = config_4_aruco_marks()

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