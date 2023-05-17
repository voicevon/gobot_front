# # from von.amq_agent import g_amq
# from von.mqtt_agent import g_mqtt,g_mqtt_broker_config
# from von.mqtt_auto_sync_var import MqttAutoSyncVar
# from twh_robot_row import TwhRobot_Row
# from gcode_sender import GcodeSender
# from statemachine import StateMachine_Item, StateMachine
# import json, time



# # How to Convert JSON Data Into a Python Object?   https://www.youtube.com/watch?v=hJ2HfejqppE 




# # class UserRequest:

# #     def __init__(self, json_string: str) -> None:
# #         if json_string == '':
# #             self.reset()
# #         else:
# #             obj = json.loads(json_string)
# #             self.robot_id = obj['dev']
# #             self.command = obj['command']
# #             self.row = obj['cell']['row']
# #             self.col = obj['cell']['col']
        
# #     def reset(self):
# #         self.robot_id = 0
# #         self.command = ''
# #         self.row = 0
# #         self.col = 0



# class TwhRobot_Layer():
#     # global g_amq
#     def __init__(self, robot_id:str) -> None:
#         self.robot_id = robot_id
#         self.row_robots = [TwhRobot_Row(0,5)]
#         for i in range(1,7):
#             new_robot = TwhRobot_Row(robot_id=robot_id, row_id=i)
#             self.row_robots.append(new_robot)

#         self.ir_state = MqttAutoSyncVar(mqtt_topic="twh/" + str(robot_id) + "/ir_state",  default_value="unknown")
#         self.ir_state.Copy_LocalToRemote()
#         self.ir_empty_count = 0
#         self.gcode_sender = GcodeSender()
#         self.current_state = 'idle'

#     def move_to(self, row:int, col:int):
#         self.row_robots[row].move_to(col)

#     def reset_current_state_to_idle(self):
#         if self.current_state != 'ready':
#             print("TwhRobot_Layer::reset_state()  Warning...." )
#         self.current_state = 'idle'

#     def spin_once(self):

#         match self.current_state:
#             case 'idle':
#                 self.check_deposit_request()


#     def eef_do_ir_check(self, row, col):
#         print('.', end = "",flush=True)
#         time.sleep(1)
#         match self.ir_state.remote_value:
#             case None:
#                 return
#             case 'unknown':
#                 # Have not get new state
#                 return

#             case "empty":
#                 # print('eef_do_ir_check() point=', 'ir_check_empty')
#                 self.ir_empty_count += 1
#                 self.ir_state.local_value = self.ir_state.default_value
#                 self.ir_state.Copy_LocalToRemote()
#                 if self.ir_empty_count < 3:
#                     is_accepted = self.eef_statemachine.stimulate('ir_check_empty', row=1, col=1)
#                 else:
#                     is_accepted = self.eef_statemachine.stimulate('ir_empty_enough', row=1, col=1)

#             case "blocked":
#                 # print('eef_do_ir_check() point=', 'ir_check_blocked')
#                 is_accepted = self.eef_stimulate('ir_check_blocked', row=1, col=1)
#                 self.ir_state.local_value = self.ir_state.default_value
#                 self.ir_state.Copy_LocalToRemote()
#                 self.ir_empty_count = 0

#             case others:
#                 print('TwhRobot_Layer::do_ir_check()', self.ir_state.remote_value)

#         if is_accepted:
#             if self.doing_job == 'withdraw':
#                 self.withdraw_request.reset()
#             else:
#                 self.deposit_request.reset()

 




# if __name__ == '__main__':
#     g_mqtt_broker_config.client_id = '20221218'
#     g_mqtt.connect_to_broker(g_mqtt_broker_config)
#     robot = TwhRobot_Layer("221109")
#     while True:
#         robot.spin_once()