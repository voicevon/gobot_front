

from von.rabbitmq_agent import g_amq, AMQ_BrokerConfig
from von.mqtt_auto_syncer import MqttAutoSyncVar
from von.mqtt_agent import g_mqtt, g_mqtt_broker_config
from gcode_sender import GcodeSender
from statemachine_item import StateMachine_Item
import json
# How to Convert JSON Data Into a Python Object?   https://www.youtube.com/watch?v=hJ2HfejqppE 
# g_amq = RabbitMqAgent()







    # def update(self):
    #     self.current_state = self.ne

class TeethWarehouseRobot():
    
    def __init__(self) -> None:
        self.ir_state = MqttAutoSyncVar("dev_221109_ir", "unknown")
        self.ir_state.auto_sync_to_local = False
        self.ir_state.auto_sync_to_remote = True
        self.gcode_sender = GcodeSender()
        self.init_statemachine()

    def init_statemachine(self):
        # for deposit
        self.machine = [StateMachine_Item('parking', 'prepare_deposit', 'feeding_centerbox', self.do_prepare_deposit)]
        self.machine.append(StateMachine_Item('feeding_centerbox', 'end_deposit', 'parking', self.do_nothing))
        self.machine.append(StateMachine_Item('feeding_centerbox', 'prepare_deposit', 'feeding_centerbox', self.do_start_deposit))
        self.machine.append(StateMachine_Item('feeding_centerbox', 'start_deposit', 'axis_x', self.do_start_deposit))
        self.machine.append(StateMachine_Item('axis_x', 'auto', 'picking_centerbox', self.do_nothing))
        self.machine.append(StateMachine_Item('picking_centerbox', 'ir_check_empty', 'picking_centerbox', self.do_pickup))
        self.machine.append(StateMachine_Item('picking_centerbox', 'ir_check_blocked', 'droping_cell', self.do_pickup))
        self.machine.append(StateMachine_Item('droping_cell', 'count_less_20', 'picking_centerbox', self.do_pickup))
        self.machine.append(StateMachine_Item('droping_cell', 'count_more_20', 'verify', self.do_pickup))
        self.machine.append(StateMachine_Item('verify', 'white', 'axis_x', self.do_pickup))
        self.machine.append(StateMachine_Item('verify', 'black', 'parking', self.do_pickup))
        # for withdraw
        self.machine.append(StateMachine_Item('parking', 'withdraw', 'picking_cell', self.do_pickup))
        self.machine.append(StateMachine_Item('picking_cell', 'ir_check_empty', 'picking_cell', self.do_ir_check, repeat_serve=True))
        self.machine.append(StateMachine_Item('picking_cell', 'ir_check_blocked', 'droping_centerbox', self.do_pickup))
        self.machine.append(StateMachine_Item('droping_centerbox', 'end_withdraw', 'be_outside', self.do_cancel))
        self.machine.append(StateMachine_Item('be_outside', 'auto', 'parking', self.do_cancel))

        #init, to enter 'parking'
        self.current_state = 'parking'
        self.do_park(True)
    
    def do_park(self, home_park: bool):
        self.gcode_sender.Eef_TurnOn_VacuumFan(False)
        if home_park:
            self.gcode_sender.home_x()
            self.gcode_sender.home_alpha()
        else:
            self.gcode_sender.move_x_to (200)
            self.gcode_sender.move_a_to(0)

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
    # g_amq = RabbitMqAgent()
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
    amq_broke_config = AMQ_BrokerConfig()
    g_amq.connect_to_broker(amq_broke_config)

    robot = TeethWarehouseRobot()

    if True:
        import time
        # g_amq = RabbitMqAgent()
        # amq_broke_config = AMQ_BrokerConfig()
        # g_amq.connect_to_broker(amq_broke_config)
        g_amq.Subscribe("twh_221109_deposit")
        g_amq.Subscribe('twh_221109_withdraw')

        while True:
            if is_dealing_deposit:
                deal_deposit()
            else:
                deal_withdraw()

            

