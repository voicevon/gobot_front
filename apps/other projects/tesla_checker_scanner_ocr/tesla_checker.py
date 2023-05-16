# importing the python open cv library
import cv2

# intialize the webcam and pass a constant which is 0
cam = cv2.VideoCapture(0)
# cam.set(3, 1280) # set the resolution
# cam.set(4, 720)
# cam.set(cv2.CAP_PROP_AUTOFOCUS, 0) # turn the autofocus off
# cam.set(cv2.CAP_PROP_BRIGHTNESS,250)
# cam.set(cv2.CAP_PROP_CONTRAST,0)
# cam.set(cv2.CAP_PROP_SATURATION, 255)
# cam.set(cv2.CAP_PROP_SETTINGS, 0)
# title of the app
cv2.namedWindow('python webcam screenshot app')

# let's assume the number of images gotten is 0
img_counter = 0
upper_y=150
lower_y= 270
height = 100
left = 70
width = 500
# while loop
while True:
    # intializing the frame, ret
    ret, frame = cam.read()
    # if statement
    if not ret:
        print('failed to grab frame')
        break
    # the frame will show with the title of test
    cv2.imshow('test', frame)
    upper = frame[upper_y:upper_y + height, left:left + width]
    lower = frame[lower_y:lower_y + height, left:left + width]
    cv2.imshow('upper',upper)
    cv2.imshow('lower',lower)
    #to get continuous live video feed from my laptops webcam
    k  = cv2.waitKey(1)
    # if the escape key is been pressed, the app will stop
    if k%256 == 27:
        print('escape hit, closing the app')
        break
    # if the spacebar key is been pressed
    # screenshots will be taken
    elif k%256  == 32:
        # the format for storing the images scrreenshotted
        img_name = f'opencv_frame_{img_counter}'
        # saves the image as a png file
        cv2.imwrite(img_name, frame)
        print('screenshot taken')
        # the number of images automaticallly increases by 1
        img_counter += 1

# release the camera
cam.release()
