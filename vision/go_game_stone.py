import cv2
import numpy

class StoneColor:
    BLANK = 0
    BLACK = 1
    WHITE = 2

class GoGameStone():

    def __init__(self):
        '''
        A stone value might be WHITE, BLACK, default is BLANK
        The value comes from cell image.
        TODO staticmethod
        '''
        self.BLANK = 0  # TODO: put this to base class
        self.BLACK = 1
        self.WHITE = 2

    def scan_black(self, cell_image, is_inspected):
        '''
        TODO: Rename to i_am_black(), return True/False
        '''
        gray = cv2.cvtColor(cell_image, cv2.COLOR_BGR2GRAY)
        blur = cv2.medianBlur(gray,7)
        ret, bin_image = cv2.threshold(blur, 50, 255, cv2.THRESH_BINARY_INV)



       
        count = cv2.countNonZero(bin_image)
        
        self.value = self.BLANK
        if count > 180: 
            self.value = self.BLACK

        return self.value

    def __detect_circles(self, cropped_img, show_processing_image=True):
        '''
        Get circle from HoughCircles().
        For better performance, some preprocess is aplied.
        '''
        gray = cv2.cvtColor(cropped_img, cv2.COLOR_BGR2GRAY)
        blur = cv2.medianBlur(gray,3)
        canny = cv2.Canny(gray,100,200)
        circles = cv2.HoughCircles(blur, method=cv2.HOUGH_GRADIENT, dp=1, minDist= 1, 
                                    minRadius=8, maxRadius=15, param1=1, param2=20)

        if circles is None:
            if show_processing_image:
                cv2.imshow('no circle origin', cropped_img)
                cv2.imshow('no circle gray', gray)
                cv2.imshow('no circle blur', blur)
                cv2.imshow('no circle canny', canny)
                cv2.waitKey(1)
        else:
            # print('^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^')
            detected_circles = numpy.uint16(numpy.around(circles))
            # print ('circles count= ', len(detected_circles))
            # if True:
            if show_processing_image:
                # draw circles
                img = cropped_img.copy()
                for (x,y,r) in detected_circles[0,:]:
                    # outer circle, is green color
                    cv2.circle(img,(x,y),r,(0,255,0),thickness=1)
                    # for showing the center of the circle, is a small , is red color
                    cv2.circle(img,(x,y),3,(0,0,255),1)
                    # print('ttttttttttttttt  x, y, r  ', x,y,r )
                cv2.imshow('circled: center lines',img)
                cv2.imshow('circled gray',gray)
                cv2.imshow('circled blur',blur)
                cv2.waitKey(1)

            if len(detected_circles) > 1:
                print('More than one circles are detected')
                cv2.waitKey(1000000)
            return detected_circles
        return None

    def scan_white(self, cell_image, is_inspected):
        '''
        TODO:  rename to i_am_whte(), return True/False
        return: 
            detected stone_color, only for White. because connected black cells have no circle
        '''
        detected_circles = self.__detect_circles(cell_image,show_processing_image=is_inspected)
        if detected_circles is None:
            if is_inspected:
                print('Inspected cell, no circles found!')
        elif len(detected_circles) == 1:
            # detected one circle. 
            height, width, depth = cell_image.shape
            mask_circle = numpy.zeros((height, width), numpy.uint8)
            
            for (x,y,r) in detected_circles[0]:
                cv2.circle(mask_circle, (x,y), radius=r, color=1, thickness=-1)
                masked_image = cv2.bitwise_and (cell_image, cell_image, mask=mask_circle)
                if is_inspected:
                    cv2.imshow('inspecting cell detected circle already', masked_image)
                    cv2.waitKey(1)
                # What color in this circle? black or white
                average_brightness = numpy.mean(masked_image)
                # print(self.__FC_RESET + 'average_brightness= %d' %average_brightness)
                if average_brightness > 30:
                    real_raduis = pow((x - width/2),2)  + pow((y-height/2),2) 
                    # print('inpseting x,y,real_raduis  ', x, y ,real_raduis) 
                    if real_raduis < 130:  # 51% of a circle can also be detected!
                        # https://stackoverflow.com/questions/20698613/detect-semicircle-in-opencv
                        # print('Positive')
                        self.value = self.WHITE
                    # else:
                    #     print('Negtive')
                    #     cv2.waitKey(100000)
                else:
                    print(self.__FC_RESET + '>>>>average_brightness= %d' %average_brightness)

        else:
           return self.value
   
