# from von.amq_agent import g_amq
from von.mqtt_agent import g_mqtt
from von.mqtt_auto_sync_var import MqttAutoSyncVar
import queue

class GcodeSender():

    def __init__(self, mqtt_topic:str) -> None:
        self.mqtt_topic = mqtt_topic
        self.sending_message_id =  MqttAutoSyncVar(mqtt_topic=mqtt_topic + "/fb", default_value=0, var_data_type=int)
        self.queue = queue.Queue()
        self.message_id = 0

    def append_gmcode_to_robot(self, gcode:str):
        payload = {}
        payload['id'] = self.message_id
        payload['gmc'] = gcode
        self.queue.put(payload)
        self.message_id += 1

    def append_command_to_app(self,command:str):
        payload = {}
        payload['id'] = self.message_id
        payload['cmd'] = command
        self.queue.put(payload)
        self.message_id += 1

    def spin_once(self):

        if self.sending_message_id.local_value != self.sending_message_id.remote_value:
            # TODO:  time out and retry
            return
        if self.queue.empty():
            return
        # queue is not empty, and got last feedback correctly.
        payload = self.queue.get()
        self.sending_message_id = payload['id']
        g_mqtt.publish(self.mqtt_topic, payload=payload)



    # def send_code(self, code):
    #     exchange_name = 'twh'
    #     queue_name = exchange_name + '_' + str(self.target_device_id) + '_gcode'  # twh_221109_gcode
    #     # if code[0:4] != 'M408':
    #     #     return
    #     if code[0:2] == 'G1':
    #         return
    #     if code[0:2] == 'G2':
    #         return
    #     if code[0:2] == 'G6':
    #         return

    #     g_amq.Publish(exchange_name, queue_name, code)

    # def home_alpha(self):
    #     self.send_code('G28a')

    # def home_y(self):
    #     self.send_code('G28Y')

    # def move_centerbox_to_outside(self):
    #     self.send_code('G1X1200')

    # def move_centerbox_to_inside(self):
    #     self.send_code('G1X400')

    # def pickup_from_centerbox(self):
    #     # move to top
    #     self.send_code('G1Z0')
    #     self.enable_vacuum_sucker(True)
    #     # arm to center position
    #     self.send_code('G6a0')
    #     # move down
    #     self.send_code('G1Z100')
    #     self.send_code('G4S2')
    #     # move up
    #     self.send_code('G1Z0')
    #     self.send_code('G4S2')
    #     # report ir_state via MQTT
    #     self.send_code('M408')

    # def pickup_from_cell(self, row, col):
    #     self.send_code("M42P18S1")
    #     self.send_code("G4S3")
    #     #report ir_state via MQTT
    #     self.send_code('M408')  
         
    # def drop_to_cellbox(self, row, col):
    #     self.move_xy_to(row * 40, col * 40)
    #     self.enable_vacuum_sucker(False)

    # def move_z_top(self):
    #     self.send_code("G1Z0")

    # def EefMove_Z_toMiddle(self, depth:int):
    #     self.send_code("G1Z30")

    # def Eef_TurnOn_VacuumFan(self, is_turn_on: bool):
    #     if is_turn_on:
    #         self.send_code('M42P33S0')
    #     else:
    #         self.send_code('M42P33S1')

    # def enable_vacuum_sucker(self, is_enable:bool):
    #     # print('gcode ','Eef_EnableSuck')
    #     if is_enable:
    #         self.send_code('M42P18S1')   # trun on
    #     else:
    #         self.send_code('M42P18S0')  # turn off
    #     self.send_code("G4S3")
            
    # def dwell(self, second: int):
    #     self.send_code('G4S' + str(second))

    # def move_arm_to_center(self):
    #     self.move_a_to(0)

    # def move_a_to(self, angle_in_rad):
    #     self.send_code("G6a" + str(angle_in_rad) )

    # def move_xy_to(self, x, y):
    #     # print('gcode ','MoveTo')
    #     self.send_code("G1X" + str(x) + "Y" + str(y))
    
    # def row_robot_move_to_col(self, row:int, col:int):
    #     mqtt_topic = 'twh/' + str(self.target_device_id) + "/row/gcode"
    #     mqtt_payload = ("G1R" + str(row) + "C" + str(col))
    #     g_mqtt.publish(mqtt_topic, mqtt_payload)