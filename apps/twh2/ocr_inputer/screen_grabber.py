# https://stackoverflow.com/questions/35097837/capture-video-data-from-screen-in-python


import numpy as np
import cv2
from mss import mss      # pip install mss
from PIL import Image

area = {'left': 0, 'top': 0, 'width': 1366, 'height': 768}

with mss() as sct:
    while True:
        screenShot = sct.grab(area)
        img = Image.frombytes('RGB', (screenShot.width, screenShot.height), screenShot.rgb)
        # img = Image.frombytes('P', (screenShot.width, screenShot.height), screenShot.rgb)

        img2 = np.array(img)
        img3 = cv2.cvtColor(img2, cv2.COLOR_BGR2RGB)
        cv2.imshow('test', np.array(img3))
        if cv2.waitKey(1) & 0xFF in (
            ord('q'), 
            27, 
        ):
            break