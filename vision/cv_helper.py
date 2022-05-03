import cv2

def  Img_RotateScale(source_image, angle_in_degree, scale=1.0):
    # rotate 180 degree, for fitting normal views.
    ''' https://www.tutorialkart.com/opencv/python/opencv-python-rotate-image/ '''
    (h, w) = source_image.shape[:2]
    center = (w / 2, h / 2)
    M = cv2.getRotationMatrix2D(center, angle=angle_in_degree, scale=scale)
    new_img = cv2.warpAffine(source_image, M, (w, h))
    return new_img