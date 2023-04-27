import numpy as np
import cv2
import pyautogui   #pip install pyautogui
import time

image = pyautogui.screenshot()
while True:
    # take screenshot using pyautogui
    
    # since the pyautogui takes as a 
    # PIL(pillow) and in RGB we need to 
    # convert it to numpy array and BGR 
    # so we can write it to the disk
    image = pyautogui.screenshot()

    image2 = cv2.cvtColor(np.array(image),
                        cv2.COLOR_RGB2BGR)
    # writing it to the disk using  opencv

    open_cv_image = np.array(image) 
    # Convert RGB to BGR 
    open_cv_image = open_cv_image[:, :, ::-1].copy()

    cv2.imwrite("image2.png", open_cv_image)
    image3 = cv2.imread("image2.png")
    cv2.imshow("captured", open_cv_image)
    if cv2.waitKey(1000) & 0xFF == ord('q'):
            break


    # time.sleep(0.1)