from von.mqtt.mqtt_agent import g_mqtt, g_mqtt_broker_config
from von.logger import Logger

# class CncSitter:
#     def __init__(self) -> None:
#         pass

#     @staticmethod
#     def move_to(x, y):
#         # send gcode
#         pass


#     @staticmethod
#     def move_to_and_take_side_pictures(x, y):
#         # send gcode
#         pass

class SitterSystem:
    MQTT_TOPIC_TAKE_PICTURE = "twh/sitter/take_picture"
    def __init__(self) -> None:
        self.state = "idle"
        g_mqtt.connect_to_broker(g_mqtt_broker_config)
        g_mqtt.subscribe("top_camera")
        g_mqtt.subscribe("pre_camera_a")
        g_mqtt.subscribe("pre_camera_b")
        g_mqtt.subscribe("pre_camera_c")
        g_mqtt.subscribe("pre_camera_f")
        g_mqtt.subscribe("post_camera_a")
        g_mqtt.subscribe("post_camera_b")
        g_mqtt.subscribe("post_camera_c")
        g_mqtt.subscribe("post_camera_f")

    def main_loop(self):
        while True:
            if self.state == "idle":
                g_mqtt.publish(self.MQTT_TOPIC_TAKE_PICTURE,  "top_camera")
                self.state = "taking_picture_top_camera"
            # read top camera, then get tooth position via opencv

            # let sitter to pickup the tooth, then move to observing area

            # take pictures , and publish to mqtt

    def onReceivedMqtt(self, topic, payload):
        if self.state == 'idle':
            Logger.Error("onReceivedMqtt(), at state=='idle'")
        if self.state == 'taking_picture_top_camera':
            if topic == 'twh/sitter/taken_picture/top_camera':
                # calculate tooth position , then move cnc eef to the position
                x, y = 123, 234
                CncSitter.move_to_and_take_side_pictures(x,y)
                self.state = 'cnc_sitter_moving_to_tooth'
        if self.state == 'cnc_sitter_moving_to_tooth':
            if topic == 'twh/sitter/cnc/at_tooth':
                # side cameras take picture
                g_mqtt.publish(self.MQTT_TOPIC_TAKE_PICTURE,"side_cameras")
                self.camera_mask = 0
                self.state = 'taking_picture_side_camera'
        if self.state == 'taking_picture_side_camera':
            if topic = 'twh/sitter/taken_picture/side_camera/XPYP':
                # save image to file
                self.camera_mask = self.camera_mask + 1
            if topic = 'twh/sitter/taken_picture/side_camera/XMYP':
                # save image to file
                self.camera_mask = self.camera_mask + 4
            if topic = 'twh/sitter/taken_picture/side_camera/XMYM':
                # save image to file
                self.camera_mask = self.camera_mask + 8
            if topic = 'twh/sitter/taken_picture/side_camera/XPYM':
                # save image to file
                self.camera_mask = self.camera_mask + 2
            if self.camera_mask == 1  #1,3,7,15,31....
                g_mqtt.publish('twh/sitter/cnc/picking_parking', '')
                self.state = 'cnc_sitter_picking_parking'
        if self.state == 'cnc_sitter_picking_parking':
            if topic=='twh/sitter/cnc/' and payload == 'parked':
                g_mqtt.publish(self.MQTT_TOPIC_TAKE_PICTURE, 'post_camera')
                self.state = 'taking_picture_post_camera'
        if self.state == 'taking_picture_post_camera':
            if topic == 'twh/sitter/taken_picture/post_camera/XPYP':
                self.camera_mask = self.camera_mask + 1
            if topic == 'twh/sitter/taken_picture/post_camera/XMYP':
                self.camera_mask = self.camera_mask + 2
            if topic == 'twh/sitter/taken_picture/post_camera/XMYM':
                self.camera_mask = self.camera_mask + 4
            if topic == 'twh/sitter/taken_picture/post_camera/XPYM':
                self.camera_mask = self.camera_mask + 8
            if self.camera_mask == 1:  #1,3,7,15,31....
                CncSitter.move_to(123,123)   # to dropping position
                self.state = 'cnc_sitter_droping_parking'
        if self.state == 'cnc_sitter_droping_parking':
            if topic=='twh/sitter/cnc/' and payload == 'parked':
                self.state = 'idle'



sitter_system = SitterSystem()
sitter_system.main_loop()

