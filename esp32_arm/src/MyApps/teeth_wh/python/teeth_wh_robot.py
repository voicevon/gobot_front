

from von.rabbitmq_agent import RabbitMqAgent, AMQ_BrokerConfig
from von.mqtt_auto_syncer import MqttAutoSyncVar
from von.mqtt_agent import g_mqtt, g_mqtt_broker_config
import json
# How to Convert JSON Data Into a Python Object?   https://www.youtube.com/watch?v=hJ2HfejqppE 
g_amq = RabbitMqAgent()


class GcodeSender():

    def __init__(self) -> None:
        self.target_device_id = 221109
        self.exchange_name = 'twh'
        self.queue_name = self.exchange_name + '_' + str(self.target_device_id) + '_gcode'

    def home_alpha(self):
        g28 = 'G28A'
        g_amq.Publish(self.exchange_name, self.queue_name, g28)

    def move_centerbox_to_outside(self):
        g1 = 'G1X1200'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)

    def move_centerbox_to_inside(self):
        g1 = 'G1X400'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)

    def pickup_from_centerbox(self):
        # move to top
        g1 = 'G1Z0'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)
        self.Eef_EnableSuck(True)
        # arm to center position
        g1 = 'G1A0'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)
        # move down
        g1 = 'G1Z100'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)
        g4 = 'G4S2'
        g_amq.Publish(self.exchange_name, self.queue_name, g4)
        # move up
        g1 = 'G1Z0'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)
        g4 = 'G4S2'
        g_amq.Publish(self.exchange_name, self.queue_name, g4)
        
    def drop_to_cellbox(self, row, col):
        self.MoveTo(row * 40, col * 40)
        self.Eef_EnableSuck(False)



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




class StateMachine_Item():

    def __init__(self, current_state:str, command:str, post_state:str, service_function, repeat_serve=False) -> None:
        self.current_state = current_state
        self.command = command
        self.post_state = post_state
        self.service_function = service_function
        self.repeat_invoke_service_function = repeat_serve

    def print_out(self):
        print("------------ StateMachine_Item() -------------")
        print("from_state=", self.current_state, "  command=", self.command, "    post_state=", self.post_state)

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
        # self.state = 'idle'
        self.ir_state = MqttAutoSyncVar("dev_221109_ir", "unknown")
        self.ir_state.auto_sync_to_local = False
        self.ir_state.auto_sync_to_remote = True
        # self.state_machine = json.load(UserRequest)
        # self.all_requests = {'withdraw_preare': UserRequest(), }
        self.init_statemachine()
        self.gcode_sender = GcodeSender()

    def init_statemachine(self):
        # self.current_state = 'idle'
        self.machine = [StateMachine_Item('idle', 'prepare_deposit', 'preparing_deposit', self.do_prepare_deposit)]
        self.machine.append(StateMachine_Item('preparing_deposit', 'prepare_deposit', 'preparing_deposit', self.do_nothing))
        self.machine.append(StateMachine_Item('preparing_deposit', 'start_deposit', 'picking_centerbox', self.do_start_deposit))
        self.machine.append(StateMachine_Item('picking_centerbox', 'ir_check_empty', 'picking_centerbox', self.do_start_deposit))
        # self.machine.append(StateMachine_Item('preparing_deposit', 'cancel', 'idle', self.do_cancel))

        self.machine.append(StateMachine_Item('idle', 'pickup', 'picking_cell', self.do_pickup))
        self.machine.append(StateMachine_Item('picking_cell', 'ir_check_pass', 'idle', self.do_ir_check, repeat_serve=True))
        self.machine.append(StateMachine_Item('picking_cell', 'ir_check_failed', 'picking_cell', self.do_pickup))
        self.machine.append(StateMachine_Item('preparing_withdraw', 'withdrawing', '', self.do_cancel))

        self.current_state = 'idle'

    def is_acceptable(self, command) ->bool:
        for item in self.machine:
            # item.print_out()
            if item.current_state == self.current_state:
                if item.command == command:
                    return True
        return False

    def statemachine_drive(self, command:str, house_cell) -> bool:


        '''
        Returns
        True:  Update to post state successed.
        False: Update to post state failed.
        '''
        for item in self.machine:
            item.print_out()
            if item.current_state == self.current_state:
                if item.command == command:
                    item.service_function(house_cell)
                    self.current_state = item.post_state
                    return True
        print("TeethWarehouseRobot::update_state() failed")
        return False 

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

    def do_nothing(self, house_cell):
        pass

    def do_prepare_deposit(self, house_cell):
        self.gcode_sender.home_alpha()
        self.gcode_sender.move_centerbox_to_outside()
    
    def do_start_deposit(self, house_cell):
        for i in range(20):
            self.gcode_sender.pickup_from_centerbox()
            # self.do_ir_check()
            self.gcode_sender.drop_to_cellbox(house_cell['row'], house_cell['col'])

    
    def do_pickup(self, house_cell):
        #get x,y from row, col
        # gcode move to x,y
        # mcode ir check
        self.ir_state.local_value = self.ir_state.default_value

    def do_cancel(self, house_cell):
        pass

    def do_ir_check(self, house_cell):
        match self.ir_state.remote_value:
            case 'unknown':
                # Have not get new state
                return

            case "passed":
                self.statemachine_drive('ir_check_pass')

            case "failed":
                self.statemachine_drive('ir_check_failed')

    # def do_deposite_user_ready(self, house_cell):
    #     self.cerebellum.Dropto_CenterBox()
    #     self.cerebellum.PickupAndCheck(house_cell)

    def SpinOnce(self):
        if self.current_state == 'picking_cell':
            self.do_ir_check(self.house_cell)
    
    def feed_request(self, request_str: str) -> bool:
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
        self.house_cell = request['cell']
        # if request.Acceptable():
        print('-------------------', request,command)
        return self.statemachine_drive(command, self.house_cell)

def test():

    g_amq = RabbitMqAgent()
    amq_broke_config = AMQ_BrokerConfig()
    g_amq.connect_to_broker(amq_broke_config)

    # img = cv2.imread("nocommand.jpg")
    # g_amq.publish_cv_image("test" , img)

    g_amq.Subscribe(queue_name='twh_221109_deposit')
    g_amq.Subscribe(queue_name='twh_221109_withdraw')
    count = 0
    while True:
        g_amq.SpinOnce()
        # time.sleep(0.9)
        # xx = g_amq.fetch_message('twh_221109_deposit')
        # # xx = ss.FetchMessage()
        # if xx is not None:
        #     print(xx)
        #     # time.sleep(1)

        xx = g_amq.fetch_message('twh_221109_withdraw')
        if xx is not None:
            if count>1:
                print("withdraw is empty................................", count)
            count =0
            print(xx)
            # time.sleep(1)
        else:
            count += 1

def deal_deposit():
    g_amq.SpinOnce()
    user_request = g_amq.fetch_message('twh_221109_deposit')
    if user_request is not None:
        robot_accepted_request = False
        while not robot_accepted_request:
            robot_accepted_request = robot.feed_request(user_request.decode('utf-8'))
            if not robot_accepted_request:
                time.sleep(1) 

def deal_withdraw():
    g_amq.SpinOnce()
    user_request = g_amq.fetch_message('twh_221109_withdraw')
    if user_request is None:
        # print('withdraw is empty ==============================================================================')
        # time.sleep(2)
        # deal_deposit = True
        pass
    else:
    #     robot_accepted_request = False
        print(user_request)
        # while not robot_accepted_request:
            # robot_accepted_request = robot.feed_request(user_request.decode('utf-8'))
            # if not robot_accepted_request:
            #     time.sleep(1) 


if __name__ == '__main__':
    # test()
    is_dealing_deposit = True

    g_mqtt.connect_to_broker(g_mqtt_broker_config)
    robot = TeethWarehouseRobot()

    if True:
        import time
        # g_amq = RabbitMqAgent()
        amq_broke_config = AMQ_BrokerConfig()
        g_amq.connect_to_broker(amq_broke_config)
        g_amq.Subscribe("twh_221109_deposit")
        g_amq.Subscribe('twh_221109_withdraw')

        while True:
            if is_dealing_deposit:
                deal_deposit()
            else:
                deal_withdraw()

            

