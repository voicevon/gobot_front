# https://stackoverflow.com/questions/35097837/capture-video-data-from-screen-in-python


import numpy as np
import cv2
from mss import mss      # pip install mss
from PIL import Image

def grab_screen():
    area = {'left': 0, 'top': 0, 'width': 1366, 'height': 768}
    with mss() as sct:
        screenShot = sct.grab(area)
        img = Image.frombytes('RGB', (screenShot.width, screenShot.height), screenShot.rgb)
        # img = Image.frombytes('P', (screenShot.width, screenShot.height), screenShot.rgb)

        img2 = np.array(img)
        img3 = cv2.cvtColor(img2, cv2.COLOR_BGR2RGB)
        cv2.imshow('origin', np.array(img3))
        cv2.waitKey(1)
        return img3


def match_template(small_image, origin):
    method = cv2.TM_SQDIFF_NORMED

    # Read the images from the file
    small_image = cv2.imread('small_image.png')
    # large_image = cv2.imread('large_image.png')

    result = cv2.matchTemplate(small_image, origin, method)

    # We want the minimum squared difference
    mn,_,mnLoc,_ = cv2.minMaxLoc(result)

    # Draw the rectangle:
    # Extract the coordinates of our best match
    MPx,MPy = mnLoc

    # Step 2: Get the size of the template. This is the same size as the match.
    trows,tcols = small_image.shape[:2]

    # Step 3: Draw the rectangle on origin
    cv2.rectangle(origin, (MPx,MPy),(MPx+tcols,MPy+trows),(0,0,255),2)

    # Display the original image with the rectangle around the match.
    cv2.imshow('output',origin)

    # The image is only displayed if we call this
    # cv2.waitKey(1)
    return MPx, MPy

def get_target_area(origin, left, top, width, height):
    cropped_image = origin[left:top, left+width:top+height]
    cv2.imshow("target", cropped_image)

    # cv2.imshow("origin", origin)
    cv2.waitKey(1)


small_image = cv2.imread('small_image.png')

while True:
    origin = grab_screen()
    # left, top = match_template(small_image, origin)

    left_offset = -20
    top_offset = 50
    width =  660
    height =  450
    # get_target_area(origin, left, top, width, height)
    y1 = top + top_offset
    x1 = left + left_offset
    y2 = y1 + height
    x2 = x1 + width
    cropped_image = origin[y1:y2, x1:x2]

    # cropped_image = origin[80:280, 150:330]
    cv2.imshow("target", cropped_image)
    if cv2.waitKey(1) & 0xFF in ( ord('q'),  27,):
        break