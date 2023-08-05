from von.mqtt.mqtt_agent import g_mqtt, g_mqtt_broker_config
from von.logger import Logger
import cv2, numpy
from arucoc_finder import ArucoFinder
from perspective_transfomer import PerspectiveTransformer

def nothing(x):pass

class SitterSystem:
    MQTT_TOPIC_CAMERA_COMMAND = "twh/sitter/camera/command"   # payload is sitter_id/camera_id 
    MQTT_TOPIC_OF_ROBOT_GCODE_BASE = 'twh/sitter/robot/gcode/'  # + sitter_id,  payload is "G1X123Y234"
    # MQTT_TOPIC_OF_CNC_PARK_BASE = 'twh/sitter/cnc/park/'      # + sitter_id, payload is ""   ??
    MQTT_TOPIC_CAMERA_RESULT = 'twh/sitter/image/'           # + sitter_id/camera_id
    MQTT_TOPIC_OF_CNC_STATE = 'twh/sitter/cnc/state/'        # + sitter_id

    CAMERA_ID_OF_TOP = "/top"
    CAMERA_ID_OF_SIDE_A = "/side/a"
    CAMERA_ID_OF_SIDE_B = "/side/b"
    CAMERA_ID_OF_BOTTOM_A = "/bottom/a"
    CAMERA_ID_OF_BOTTOM_B = "/bottom/b"



    STATE_IDLE = 'STATE_IDLE'
    STATE_TAKING_PICTURE_TOP_CAMERA = 'taking_picture_top_camera'
    STATE_TAKING_PICTURE_SIDE_CAMERA = 'taking_picture_side_camera'
    STATE_TAKING_PICTURE_PARK_CAMERA = 'taking_picture_park_camera'
    STATE_CNC_MOVING_TO_TOOTH =  'cnc_sitter_moving_to_tooth'
    STATE_CNC_PARKING = 'cnc_sitter_droping_parking'


    def __init__(self, sitter_id:str, starting_sequence_counter:int) -> None:
        self.state = self.STATE_IDLE

        g_mqtt.append_on_received_message_callback(self.onReceivedMqtt)
        g_mqtt.subscribe(self.MQTT_TOPIC_CAMERA_RESULT + "#")
        g_mqtt.subscribe(self.MQTT_TOPIC_OF_CNC_STATE + "#")

        self.aruco_finder = ArucoFinder([49,48,15,13])
        self.sitter_id = sitter_id
        self.sequence_counter = starting_sequence_counter
        # cv2.namedWindow('videoUI', cv2.WINDOW_NORMAL)
        # cv2.createTrackbar('T','videoUI',0,255, nothing)

    def main_loop(self):
        while True:
            if self.state == self.STATE_IDLE:
                payload = self.sitter_id + "/top"
                g_mqtt.publish(self.MQTT_TOPIC_CAMERA_COMMAND, payload )
                Logger.Info("published " + self.MQTT_TOPIC_CAMERA_COMMAND + "    " + payload)
                self.state = self.STATE_TAKING_PICTURE_TOP_CAMERA

            # read top camera, then get tooth position via opencv

            # let sitter to pickup the tooth, then move to observing area

            # take pictures , and publish to mqtt



    def save_jpg_file(self, file_path, mqtt_payload):
        # Logger.Info("onReceivedMqtt,   topic= " + topic)
        jpg_file = open(file_path, "wb")
        jpg_file.write(mqtt_payload)
        jpg_file.close()

        nparr = numpy.frombuffer(mqtt_payload, numpy.uint8)
        origin_image = cv2.imdecode(nparr, cv2.IMREAD_COLOR) # cv2.IMREAD_COLOR in OpenCV 3.1
        origin_iamge_smaller = cv2.resize(origin_image, (300,200), interpolation=cv2.INTER_AREA)
        cv2.imshow("origin", origin_iamge_smaller)
        cv2.waitKey(1)

    def debug_show_image(self, window_name, image):

        scale_percent = 20 # percent of original size
        width = int(image.shape[1] * scale_percent / 100)
        height = int(image.shape[0] * scale_percent / 100)
        dim = (width, height)
        # resize image
        small_image = cv2.resize(image, dim, interpolation = cv2.INTER_AREA)
        
        cv2.imshow(window_name, small_image)
        cv2.waitKey(1)

    def onReceivedMqtt(self, topic, payload):
        if self.state == self.STATE_IDLE:
            Logger.Error("onReceivedMqtt(), at state=='idle'")
        if self.state == self.STATE_TAKING_PICTURE_TOP_CAMERA:
            if topic ==  self.MQTT_TOPIC_CAMERA_RESULT + self.sitter_id + "/top":
                nparr = numpy.frombuffer(payload, numpy.uint8)
                orgin_image = cv2.imdecode(nparr, cv2.IMREAD_COLOR) # cv2.IMREAD_COLOR in OpenCV 3.1
                self.debug_show_image("origin", orgin_image)
                cv2.waitKey(1)
                perspectived_iamge =  self.ToPerspective(orgin_image, print_report=True)
                Logger.Print("shape", perspectived_iamge.shape)
                self.debug_show_image("perspectived", perspectived_iamge)
                cv2.waitKey(1)
                cropped_image = self.ToCroppedBinary(perspectived_iamge)
                self.debug_show_image("cropped", cropped_image)
                cv2.waitKey(1)
                x,y = self.GetCenterPoint(cropped_image)
                payload = "G1X" + str(x) + "Y" + str(y)
                g_mqtt.publish(self.MQTT_TOPIC_OF_ROBOT_GCODE_BASE + self.sitter_id, payload)
                cv2.waitKey(1)
                self.state = self.STATE_CNC_MOVING_TO_TOOTH
        if self.state == self.STATE_CNC_MOVING_TO_TOOTH:
            if topic == self.MQTT_TOPIC_OF_CNC_STATE and payload == 'arrived':
                # side cameras take picture
                g_mqtt.publish(self.MQTT_TOPIC_CAMERA_COMMAND,"side_cameras")
                self.camera_mask = 0
                self.state = self.STATE_TAKING_PICTURE_SIDE_CAMERA
        if self.state == self.STATE_TAKING_PICTURE_SIDE_CAMERA:
            if topic == 'twh/sitter/taken_picture/side_camera/XPYP':
                # save image to file
                self.save_jpg_file("d2325/t01/s" + str(self.sequence_counter) +"_xpyp.jpg", payload)
                self.camera_mask = self.camera_mask + 1
            if topic == 'twh/sitter/taken_picture/side_camera/XMYP':
                # save image to file
                self.camera_mask = self.camera_mask + 4
            if topic == 'twh/sitter/taken_picture/side_camera/XMYM':
                # save image to file
                self.camera_mask = self.camera_mask + 8
            if topic == 'twh/sitter/taken_picture/side_camera/XPYM':
                # save image to file
                self.camera_mask = self.camera_mask + 2
            if self.camera_mask == 1:    #1,3,7,15,31....
                g_mqtt.publish('twh/sitter/cnc/picking_parking', '')
                self.state = 'cnc_sitter_picking_parking'
        if self.state == 'cnc_sitter_picking_parking':
            if topic=='twh/sitter/cnc/' and payload == 'parked':
                g_mqtt.publish(self.MQTT_TOPIC_CAMERA_COMMAND, 'post_camera')
                self.state = self.STATE_TAKING_PICTURE_PARK_CAMERA
        if self.state == self.STATE_TAKING_PICTURE_PARK_CAMERA:
            if topic == 'twh/sitter/taken_picture/post_camera/XPYP':
                self.save_jpg_file("d2325/t01/b" + str(self.sequence_counter) +"_xpyp.jpg", payload)
                self.camera_mask = self.camera_mask + 1
            if topic == 'twh/sitter/taken_picture/post_camera/XMYP':
                self.camera_mask = self.camera_mask + 2
            if topic == 'twh/sitter/taken_picture/post_camera/XMYM':
                self.camera_mask = self.camera_mask + 4
            if topic == 'twh/sitter/taken_picture/post_camera/XPYM':
                self.camera_mask = self.camera_mask + 8
            if self.camera_mask == 1:  #1,3,7,15,31....
                payload = "G1X100Y100"  # park for bottom camera.
                g_mqtt.publish(self.MQTT_TOPIC_OF_ROBOT_GCODE_BASE, payload=payload)
                self.state = self.STATE_CNC_PARKING
                self.sequence_counter =  self.sequence_counter + 1

        if self.state == self.STATE_CNC_PARKING:
            if topic== self.MQTT_TOPIC_OF_CNC_STATE and payload == 'parked':
                self.state = self.STATE_IDLE

    def test(self):
        image = cv2.imread("test.png")
        # cv2.imshow("test", image)
        # cv2.waitKey(3000)
        perspectived_image =  self.ProcessOriginImage(image, print_report= True)
        cv2.imshow("image", perspectived_image)
        cv2.waitKey(9000)

    def GetCenterPoint(self, binary_image):
        # try to get center position
        # https://stackoverflow.com/questions/65930839/getting-a-centre-of-an-irregular-shape
        thresh = cv2.morphologyEx(binary_image, cv2.MORPH_DILATE, numpy.ones((3, 3)))
        contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        contours = [c for c in contours if cv2.contourArea(c) > 100]
        (x, y), r = cv2.minEnclosingCircle(contours[0])
        output = thresh.copy()
        cv2.circle(output, (int(x), int(y)), 3, (0, 0, 0), -1)
        cv2.putText(output, f"{(int(x), int(y))}", (int(x-50), int(y-10)), cv2.FONT_HERSHEY_PLAIN, 5, (255, 0, 0), 3)
        cv2.circle(output, (int(x), int(y)), int(r), (255, 0, 0), 2)
        return x,y


    def ToCroppedBinary(self, perspectived_image):
        left, width, top, height = 40,750,98,900
        cropped_image = perspectived_image[top:top+height, left:left+width]   #image[start_x:end_x, start_y:end_y]
        # detect circles
        gray_iamge = cv2.cvtColor(cropped_image, cv2.COLOR_BGR2GRAY)
        blur = cv2.medianBlur(gray_iamge,3)
        # canny = cv2.Canny(gray_iamge,100,200)
        # circles = cv2.HoughCircles(blur, method=cv2.HOUGH_GRADIENT, dp=1, minDist= 1, 
        #                             minRadius=8, maxRadius=15, param1=1, param2=20)
        # self.debug_show_image ("cropped", cropped_image)

        # thresh = cv2.getTrackbarPos('T','videoUI')
        thresh = 188
        binary_image = cv2.threshold(blur, thresh, 255, cv2.THRESH_BINARY)[1]

        return binary_image
    
    def ToPerspective(self, origin_image, print_report:bool):
        '''
        * Return false, If could not detect all known aruco marks. 
        ### After this processing,  Below properties will be set.
        * self.all_marks
        * self.perspectived_image
        * self.house_vender_image (is perspectived, and cropped)
        * self.board_image(is perspectived, and cropped)
        '''
        self.all_marks = self.aruco_finder.ScanMarks(origin_image=origin_image,print_report=True)
        if self.all_marks is None:
            print('[Warn] ProcessOriginImage(), ScanMarks() returns None')
            return None
        
        mark_points_in_origin = self.aruco_finder.GetPoints_For_PespectiveInput(mark_ids=[48,13,49,15])  
        if mark_points_in_origin is None:
            print('[Warn] ProcessOriginImage(), GetPoints_For_PespectiveInput() returns bad')
            return None
        
        transformer = PerspectiveTransformer()
        
        perspectived_image = transformer.get_perspective_view(origin_image, mark_points_in_origin)
        # ImageLogger.Output("perspectived_image", self.perspectived_image, to_where=ImageLoggerToWhere.TO_SCREEN)
        return perspectived_image


g_mqtt_broker_config.client_id ="2023-0801"
g_mqtt.connect_to_broker(g_mqtt_broker_config,blocked_connection=True)
sitter_system = SitterSystem("2325", 10000)
sitter_system.main_loop()
# sitter_system.test()

