# importing the python open cv library
import cv2

# intialize the webcam and pass a constant which is 0
cam = cv2.VideoCapture(1)
# cam.set(3, 1280) # set the resolution
# cam.set(4, 720)
# cam.set(cv2.CAP_PROP_AUTOFOCUS, 0) # turn the autofocus off
# cam.set(cv2.CAP_PROP_BRIGHTNESS,250)
# cam.set(cv2.CAP_PROP_CONTRAST,0)
# cam.set(cv2.CAP_PROP_SATURATION, 255)
# cam.set(cv2.CAP_PROP_SETTINGS, 0)



while True:
    # intializing the frame, ret
    ret, frame = cam.read()
    # if statement
    if not ret:
        print('failed to grab frame')
        break
    # the frame will show with the title of test
    cv2.imshow('test', frame)
    cv2.waitKey(1)

# release the camera
cam.release()
