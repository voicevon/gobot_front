from vision.robot_eye import MonoEye

import cv2
import numpy as np


class GarmentBot_States:
    Working =1
    Charging =2
    Sleeping =4


class vision:
    def __init__(self) -> None:
        pass
    
    def find_corners(self, image):
        '''
        marker_ids is a list of [top_right, bottom_right, bottome_left, top_left]
        This code can not find any id which is greater than 250.
        So the maximum id can be used is 249. saying the range is 0 to 249
        '''
        arucoDict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_250)
        arucoParams = cv2.aruco.DetectorParameters_create()
        corners, ids, rejected = cv2.aruco.detectMarkers(image, arucoDict, parameters=arucoParams)
        # result = []
        print('GridFinder.find_corners()  double check target ids ',self.__mark_ids)
        print('GridFinder.find_corners()  found ids ', ids)
        result = []
        # verify *at least* one ArUco marker was detected
        if len(corners) >= len(self.__mark_ids):
            if config.publish_mqtt:
                debug_image = image.copy()
            # get rid of useless corners

            # flatten the ArUco IDs list
            ids = ids.flatten()
            current_corner_index = 1
            for target_id in self.__mark_ids:
                #print('----------------Searching.... ', target_id)
                # loop over the detected ArUCo corners
                for (markerCorner, markerID) in zip(corners, ids):
                    if target_id == markerID:
                        #print('got matched id', target_id)
                        # extract the marker corners (which are always returned in order of:
                        #   [top-left, top-right, bottom-right, and bottom-left]
                        corners2 = markerCorner.reshape((4, 2))
                        topLeft, topRight, bottomRight, bottomLeft = corners2
                        # convert each of the (x, y)-coordinate pairs to integers
                        topRight = (int(topRight[0]), int(topRight[1]))
                        bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
                        bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
                        topLeft = (int(topLeft[0]), int(topLeft[1]))    
                        if len(self.__mark_ids) == 4:
                            if current_corner_index == 1:
                                # top left corner of the plate, we take bottom right point of the marker.
                                result.append(bottomLeft)
                            if current_corner_index == 2:
                                # bottom right corner of the plate, we take top right point of the marker.
                                result.append(bottomRight)
                            if current_corner_index == 3:
                                # bottom left corner of the plate, we take top left point of the marker
                                result.append(topRight)
                            if current_corner_index == 4:
                                # top left corner of the plate, we take top bottom point of the marker
                                result.append(topLeft)

                        if len(self.__mark_ids) == 2:
                            # keep the result in anticlockwise.
                            if current_corner_index == 1:
                                # right mark of the plate, we append two left points of the marker
                                result.append(topLeft)
                                result.append(bottomLeft)
                            if current_corner_index == 2:
                                # left mark of the plate, we append two right points of the marker
                                result.append(bottomRight)
                                result.append(topRight)


                        # print("[INFO] ArUco marker ID: {}".format(markerID))
                        # current_corner_index += 1

                        if config.publish_mqtt:
                            # compute and draw the center (x, y) - coordinates of the ArUco marker
                            cX = int((topLeft[0] + bottomRight[0]) / 2.0)
                            cY = int((topLeft[1] + bottomRight[1]) / 2.0)
                            # print('markid=', markerID, 'center=', (cX, cY),topLeft, bottomRight, bottomLeft, topLeft)
                            # draw the bounding box of the ArUCo detection
                            if current_corner_index == 1:
                                color = (0,0,255)
                                pen_thickness = 5
                            else:
                                color = (0,255,0)
                                pen_thickness = 2
                            cv2.line(debug_image, topLeft, topRight, color, pen_thickness)
                            cv2.line(debug_image, topRight, bottomRight, color, pen_thickness)
                            cv2.line(debug_image, bottomRight, bottomLeft, color, pen_thickness)
                            cv2.line(debug_image, bottomLeft, topLeft, color, pen_thickness)

                            cv2.circle(debug_image, (cX, cY), 4, (0, 0, 255), -1)
                            # draw the ArUco marker ID on the image
                            cv2.putText(debug_image, str(markerID),
                                        (topLeft[0], topLeft[1] - 15), cv2.FONT_HERSHEY_SIMPLEX,
                                        8, (0, 255, 0), 2)

                            # img_marker = self.draw_axis(image,0,0,0)


                            # show the output image
                    
                            # rvec, tvec = cv2.aruco.estimatePoseSingleMarkers(corners, 0.05, mtx, dist) #Estimate pose of each marker and return the values rvet and tvec---different from camera coeficcients  
                            # (rvec-tvec).any() # get rid of that nasty numpy value array error  
                            
                            # cv2.aruco.drawAxis(image, mtx, dist, rvec, tvec, 0.1) #Draw Axis  
                            # cv2.aruco.drawAxis(image, )
                            # cv2.aruco.drawDetectedMarkers(image, corners) #Draw A square around the markers  

                            # image = cv2.aruco.drawMarker(cv2.aruco.DICT_4X4_1000,)
                            # image = self.draw_axis_2(image, corners)
                        current_corner_index += 1
            if config.publish_mqtt:
                g_mqtt.publish_cv_image('gobot/image/grid/aruco', debug_image)
        return result

    def detect_grid_from_aruco_corners(self, origin_image):
        # Get corners position from detecting aruco marks
        corners = self.find_corners(origin_image)
        # print(corners)
        if corners != None:
            if len(corners) >= len(self.__mark_ids):
                # Get perspectived image
                perspect_img = self.get_perspective_view(origin_image,corners)
                return perspect_img
        return None


class HardwareCommu:
    def __init__(self) -> None:
        self.Mega328_I2c_Addr = 0x3f
        self.Pwm_I2C_Addr = 0x2c
        self.left_wheel_motor_pin_a = 2
        self.left_wheel_motor_pin_b =3
        self.right_wheel_motor_pin_a = 4
        self.right_wheel_motor_pin_b =5
        self.vertica_motor_pin_a =5
        self.verical_motor_pin_b = 4
        self.angle_motor_pin_a = 2
        self.angle_motor_pin_b = 3

        #init i2c_bus
        self.i2c_bus = 1
        # init pwm driver
        self.pwm =1

        #init adc reader
        self.adc =1 

    def Write_through_mega328(self):
        bytes = 1 << self.left_wheel_motor_pin_a 
        bytes += 1 << self.left_wheel_motor_pin_b
        bytes += 1 << self.right_wheel_motor_pin_a
        bytes += 1<< self.right_wheel_motor_pin_b
        bytes += 1<< self.verical_motor_pin_b
        bytes += 1 << self.vertica_motor_pin_a
        bytes += 1 << self.angle_motor_pin_a
        bytes += 1 << self.angle_motor_pin_b

        self.i2c_bus.write(bytes)

    def Write_pwm(self, motor_id, pwm_value):
        self.pwm.write(pwm_value)

    def Read_battery_voltage(self):
        return 0

    def Read_charger_voltage(self):
        return 0


class DcMotor:
    def __init__(self, motor_id) -> None:
        self.motor_id = motor_id
        self.hardware = HardwareCommu()
    
    def SetSpeed(self, logical_speed):
        pwm_speed = 200 + logical_speed
        self.hardware.Write_pwm(self.motor_id, pwm_speed)
        

    def SetDirection(self, new_direction):
        if new_direction == CW:
            self.hardware.
            self.hardware.Write_through_mega328()
        else:
            self.i2c_bus.write(234)



class AGV:
    def __init__(self) -> None:
        self.left_wheel = DcMotor
        self.right_wheel = DcMotor
        self.i2c_bus = 0
        self.eye = MonoEye()



    def Move(self):
        # Read track error from camera
        self.eye.take_picture()
        
        track_error = 0
        self.left_wheel.SetSpeed(30 + track_error)
        self.right_wheel.SetSpeed(30 - track_error)

    def Stop(self):
        self.left_wheel.SetSpeed(0)
        self.right_wheel.SetSpeed(0)

    def SetLight(self, mode):
        if True:
            # Sleeping
            # set light breath.
            gpio.write(11)

class GarmentBot:
    def __init__(self) -> None:
        self.state = GarmentBot_States.Sleeping
        self.agv = AGV()


    def Start(self):
        self.state = GarmentBot_States.Working

    def Action_onWorking(self):
        if True:
            # Got obstacle
            self.agv.Stop()
        else:
            # No obsctacle
            self.agv.Move()


    def Action_onBatteryCharging(self):
        if True:
            # Battery is full
            self.state = GarmentBot_States.Sleeping

    def Action_onSleeping(self):
        self.agv.SetLight(breath)

    def SpinOnce(self):
        actions = {GarmentBot_States.Charging: self.Action_onBatteryCharging,
                    GarmentBot_States.Sleeping: self.Action_onSleeping,
                    GarmentBot_States.Working: self.Action_onWorking,
        }
        myFunction = actions[self.state]
        myFunction()

        


if __name__ == "__MAIN__":
    mybot = GarmentBot()
    mybot.Start()
    while True:
        mybot.SpinOnce()