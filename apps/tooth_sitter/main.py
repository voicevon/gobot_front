from von.mqtt.mqtt_agent import g_mqtt, g_mqtt_broker_config
from von.logger import Logger
import cv2, numpy
from arucoc_finder import ArucoFinder
from perspective_transfomer import PerspectiveTransformer



class SitterSystem:
    MQTT_TOPIC_TAKE_PICTURE_COMMAND = "twh/sitter/take_picture/command"  # payload is sitter_id/camera_id 
    MQTT_TOPIC_TAKE_PICTURE_RESULT = 'twh/sitter/image/#'   # + sitter_id/camera_id
    # MQTT_TOPIC_TAKE_PICTURE_RESULT = 'test'   # + sitter_id/camera_id
    MQTT_TOPIC_OF_CNC_COMMAND = 'twh/sitter/cnc/command'    #  payload is sitter_id
    MQTT_TOPIC_OF_CNC_STATE = 'twh/sitter/cnc/state/#'        # + sitter_id

    STATE_IDLE = 'STATE_IDLE'
    STATE_TAKING_PICTURE_TOP_CAMERA = 'taking_picture_top_camera'
    STATE_TAKING_PICTURE_SIDE_CAMERA = 'taking_picture_side_camera'
    STATE_TAKING_PICTURE_PARK_CAMERA = 'taking_picture_park_camera'
    STATE_CNC_MOVING_TO_TOOTH =  'cnc_sitter_moving_to_tooth'
    STATE_CNC_PARKING = 'cnc_sitter_droping_parking'


    def __init__(self) -> None:
        self.state = self.STATE_IDLE
        g_mqtt_broker_config.client_id ="2023-0801"
        g_mqtt.connect_to_broker(g_mqtt_broker_config)
        g_mqtt.append_on_received_message_callback(self.onReceivedMqtt)

        g_mqtt.subscribe(self.MQTT_TOPIC_TAKE_PICTURE_RESULT)
        g_mqtt.subscribe(self.MQTT_TOPIC_OF_CNC_STATE)
        self.aruco_finder = ArucoFinder([49,48,15,13])


    def main_loop(self):
        while True:
            if self.state == self.STATE_IDLE:
                g_mqtt.publish(self.MQTT_TOPIC_TAKE_PICTURE_COMMAND,  "2325/top")
                self.state = self.STATE_TAKING_PICTURE_TOP_CAMERA
                Logger.Info("published " + self.MQTT_TOPIC_TAKE_PICTURE_COMMAND)

            # read top camera, then get tooth position via opencv

            # let sitter to pickup the tooth, then move to observing area

            # take pictures , and publish to mqtt

    def onReceivedMqtt(self, topic, payload):
        Logger.Info("onReceivedMqtt,   topic= " + topic)
        jpg_file = open("aaaa.jpg", "wb")
        jpg_file.write(payload)
        jpg_file.close()

        nparr = numpy.fromstring(payload, numpy.uint8)
        img_np = cv2.imdecode(nparr, cv2.IMREAD_COLOR) # cv2.IMREAD_COLOR in OpenCV 3.1
        cv2.imshow("test", img_np)
        cv2.waitKey(1)



        if self.state == self.STATE_IDLE:
            Logger.Error("onReceivedMqtt(), at state=='idle'")
        if self.state == self.STATE_TAKING_PICTURE_TOP_CAMERA:
            if topic == 'twh/sitter/taken_picture/top_camera':
                # calculate tooth position , then move cnc eef to the position
                payload = {}
                payload['X'] = 123
                payload['y'] = 345
                payload['command'] = 'move_to_tooth'
                g_mqtt.publish(self.MQTT_TOPIC_OF_CNC_COMMAND, payload=payload)
                self.state = self.STATE_CNC_MOVING_TO_TOOTH
        if self.state == self.STATE_CNC_MOVING_TO_TOOTH:
            if topic == self.MQTT_TOPIC_OF_CNC_STATE and payload == 'arrived_tooth':
                # side cameras take picture
                g_mqtt.publish(self.MQTT_TOPIC_TAKE_PICTURE_COMMAND,"side_cameras")
                self.camera_mask = 0
                self.state = self.STATE_TAKING_PICTURE_SIDE_CAMERA
        if self.state == self.STATE_TAKING_PICTURE_SIDE_CAMERA:
            if topic == 'twh/sitter/taken_picture/side_camera/XPYP':
                # save image to file
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
                g_mqtt.publish(self.MQTT_TOPIC_TAKE_PICTURE_COMMAND, 'post_camera')
                self.state = self.STATE_TAKING_PICTURE_PARK_CAMERA
        if self.state == self.STATE_TAKING_PICTURE_PARK_CAMERA:
            if topic == 'twh/sitter/taken_picture/post_camera/XPYP':
                self.camera_mask = self.camera_mask + 1
            if topic == 'twh/sitter/taken_picture/post_camera/XMYP':
                self.camera_mask = self.camera_mask + 2
            if topic == 'twh/sitter/taken_picture/post_camera/XMYM':
                self.camera_mask = self.camera_mask + 4
            if topic == 'twh/sitter/taken_picture/post_camera/XPYM':
                self.camera_mask = self.camera_mask + 8
            if self.camera_mask == 1:  #1,3,7,15,31....
                payload = {}
                payload['command'] = 'park'
                g_mqtt.publish(self.MQTT_TOPIC_OF_CNC_COMMAND, payload=payload)
                self.state = self.STATE_CNC_PARKING
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


    def ProcessOriginImage(self, origin_image, print_report:bool):
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
        mark_points = self.aruco_finder.GetPoints_For_PespectiveInput(mark_ids=[48,13,49,15])
        if mark_points is None:
            print('[Warn] ProcessOriginImage(), GetPoints_For_PespectiveInput() returns bad')
            return None
        transformer = PerspectiveTransformer()
        perspectived_image = transformer.get_perspective_view(origin_image, mark_points)
        # ImageLogger.Output("perspectived_image", self.perspectived_image, to_where=ImageLoggerToWhere.TO_SCREEN)
        return perspectived_image

sitter_system = SitterSystem()
sitter_system.main_loop()
# sitter_system.test()

