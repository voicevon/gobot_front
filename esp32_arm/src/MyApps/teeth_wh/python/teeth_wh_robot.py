from von.rabbitmq_agent import RabbitMqAgent, AMQ_BrokerConfig
from von.mqtt_auto_syncer import MqttAutoSyncVar
from von.mqtt_agent import g_mqtt, g_mqtt_broker_config
import json
# How to Convert JSON Data Into a Python Object?   https://www.youtube.com/watch?v=hJ2HfejqppE 


class GcodeSender():

    def __init__(self) -> None:
        self.target_device_id = 221109
        self.exchange_name = 'twh'
        self.queue_name = self.exchange_name + '_' + str(self.target_device_id) + '_gcode'

    def EefMove_Z_toTop(self):
        '''
        For Ir checking product
        '''
        print('gcode ','EefMove_Z_toTop')
        pass

    def EefMove_Z_toMiddle(self, depth:int):
        '''
        For 
        '''
        print('gcode ','EefMove_Z_toMiddle')
        pass

    def Eef_TurnOn_VacuumFan(self, is_turn_on: bool):
        print('gcode ','Eef_TurnOn_VacuumFan')
        pass

    def Eef_EnableSuck(self, is_enable:bool):
        print('gcode ','Eef_EnableSuck')
        pass

    def PauseForWaiting(self, second: int):
        print('gcode ','PauseForWaiting')
        pass


    def MoveArmToCenter(self):
        print('gcode ','MoveArmToCenter')
        g1 = "G1A0"


    def MoveTo(self, x, y):
        print('gcode ','MoveTo')
        g1 = "G!X" + str(x) + "Y" + str(y)
        g_amq.Publish(self.exchange_name, self.queue_name, g1)

class RobotCerebellum():
    def __init__(self) -> None:
        # self.map = RobotMap()
        self.gcode_sender = GcodeSender()
        pass

    def GetXY_FromRowCol(self, row, col):
        x = row * 40
        y = col * 40
        return x,y

    def PickupAndCheck(self, row: int, col: int):
        self.gcode_sender.EefMove_Z_toTop()
        x,y = self.GetXY_FromRowCol(row=row, col=col)
        self.gcode_sender.MoveTo(x,y)
        self.gcode_sender.EefMove_Z_toMiddle(12)
        self.gcode_sender.Eef_EnableSuck(True)
        self.gcode_sender.EefMove_Z_toTop()

    def Dropto_CenterBox(self):
        self.gcode_sender.MoveArmToCenter()
        self.gcode_sender.Eef_EnableSuck(False)  # drop the tooth to out-box
        self.gcode_sender.PauseForWaiting(3)
        self.gcode_sender.Eef_EnableSuck(False)

class StateMachine_Item():

    def __init__(self, current_state:str, command:str, post_state:str, service_function) -> None:
        self.command = command
        self.current_state = current_state
        self.post_state = post_state
        self.service_function = service_function

    @classmethod
    def from_json(cls, json_string):
        json_dict = json.loads(json_string)
        return cls(** json_dict)

    def is_acceptable(self,  current_state: str) -> bool:
        if current_state in self.current_state:
            return True
        return False

    # def update(self):
    #     self.current_state = self.ne

class TeethWarehouseRobot():
    def __init__(self) -> None:
        self.cerebellum = RobotCerebellum()
        # self.state = 'idle'
        self.ir_state = MqttAutoSyncVar("dev_221109_ir", "unknown")
        self.ir_state.auto_sync_to_local = False
        self.ir_state.auto_sync_to_remote = True
        # self.state_machine = json.load(UserRequest)
        # self.all_requests = {'withdraw_preare': UserRequest(), }
        self.init_statemachine()

    def init_statemachine(self):
        # self.current_state = 'idle'
        self.machine = [StateMachine_Item('idle', 'prepare_deposit', 'preparing_deposit', self.do_prepare_deposit)]
        # self.machine.append(StateMachine_Item('idle', 'prepare_withdraw', 'preparing_withdraw', self.do_prepare_withdraw))
        # self.machine.append(StateMachine_Item('preparing_withdraw', 'cancel', 'idle', self.do_cancel))
        self.machine.append(StateMachine_Item('preparing_deposit', 'cancel', 'idle', self.do_cancel))
        # self.machine.append(StateMachine_Item('preparing_deposit', 'cancel', 'idle', self.do_cancel))

        self.current_state = 'idle'


    def is_acceptable(self, command) ->bool:
        for item in self.machine:
            if item.current_state == self.current_state:
                if item.command == command:
                    return True
        return False

    def update_state(self, command:str) -> bool:
        '''
        Returns
        True:  Update to post state successed.
        False: Update to post state failed.
        '''
        for item in self.machine:
            if item.current_state == self.current_state:
                if item.command == command:
                    item.service_function()
                    self.current_state = item.post_state
                    return True
        print("TeethWarehouseRobot::update_state() failed")
        return False 

    def do_prepare_deposit(self):
        pass
    
    def do_prepare_withdraw(self):
        pass

    def do_cancel(self):
        self.current_state = 'idle'
        pass

    def SpinOnce(self):
        pass

    
    def AppendCommand(self, request_str: str) -> bool:
        '''
        This should start a new thread.
        Reason: Dynamic gcode follow a  branched processing flow. Synced with hardware robot. 
                And take a long time up to couple minutes.
        Return:
            True: The command is accepted by state machine
            False: The commnd is rejected by state machine
        '''
        request = json.loads(request_str)
        command = request['command']
        # if request.Acceptable():
        self.update_state(command)





if __name__ == '__main__':
    g_mqtt.connect_to_broker(g_mqtt_broker_config)
    robot = TeethWarehouseRobot()

    if True:
        import time
        g_amq = RabbitMqAgent()
        amq_broke_config = AMQ_BrokerConfig()
        g_amq.connect_to_broker(amq_broke_config)
        g_amq.Subscribe("twh_221109_request")
        
        while True:
            g_amq.SpinOnce()
            user_request = g_amq.FetchMessage()
            if user_request is not None:
                robot_accepted_request = False
                while not robot_accepted_request:
                    robot.AppendCommand(user_request.decode('utf-8'))
                    time.sleep(0.1) 
            

