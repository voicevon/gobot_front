import cv2
# https://www.youtube.com/watch?v=vXqKniVe6P8

class AppWindowIdentifier:
    def __init__(self) -> None:
        self.__config = {}

    def match_template(self, origin):
        method = cv2.TM_SQDIFF_NORMED

        result = cv2.matchTemplate(self.__config["marker_image"], origin, method)
        # We want the minimum squared difference
        #  mn,_,mnLoc,_ = cv2.minMaxLoc(result)
        min_value,max_value,minLocation,max_location = cv2.minMaxLoc(result)
        # Draw the rectangle:
        # Extract the coordinates of our best match
        MPx, MPy = minLocation

        imshow = False
        if imshow:
            # Step 2: Get the size of the template. This is the same size as the match.
            # trows, tcols = self.__config["marker_image"].shape[:2]
            width = self.__config["marker_image"][1]
            height = self.__config["marker_image"][0]
            # Step 3: Draw the rectangle on origin
            # cv2.rectangle(origin, (MPx,MPy),(MPx+tcols,MPy+trows),(0,0,255),2)
            cv2.rectangle(origin, max_location, (max_location[0] + width, max_location[1] + height),(0,0,255),2)
            cv2.imshow('match template',origin)
            cv2.waitKey(1)
        return MPx, MPy