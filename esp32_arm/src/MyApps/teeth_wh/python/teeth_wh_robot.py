from von.rabbitmq_agent import g_amq, AMQ_BrokerConfig
from von.mqtt_auto_syncer import MqttAutoSyncVar
from von.mqtt_agent import g_mqtt, g_mqtt_broker_config
from gcode_sender import GcodeSender
from statemachine_item import StateMachine_Item
import json
# How to Convert JSON Data Into a Python Object?   https://www.youtube.com/watch?v=hJ2HfejqppE 


class TeethWarehouseRobot():
    
    def __init__(self) -> None:
        self.ir_state = MqttAutoSyncVar("twh/221109/ir_state", "unknown")
        self.ir_state.auto_sync_to_local = False
        self.ir_state.auto_sync_to_remote = True
        self.gcode_sender = GcodeSender()
        self.init_statemachine()
        

    def init_statemachine(self):
        # for deposit
        self.machine = [StateMachine_Item('parking', 'prepare_deposit', 'feeding_centerbox', self.do_feed_centerbox)]
        self.machine.append(StateMachine_Item('feeding_centerbox', 'end_deposit', 'parking', self.do_park))
        self.machine.append(StateMachine_Item('feeding_centerbox', 'prepare_deposit', 'feeding_centerbox', self.do_nothing))
        self.machine.append(StateMachine_Item('feeding_centerbox', 'start_deposit', 'axis_x', self.do_position_x_for_cell))
        self.machine.append(StateMachine_Item('axis_x', 'auto', 'picking_centerbox', self.do_pick_centerbox))
        self.machine.append(StateMachine_Item('picking_centerbox', 'ir_check_blocked', 'droping_cell', self.do_drop_cell))
        self.machine.append(StateMachine_Item('picking_centerbox', 'ir_check_empty', 'picking_centerbox', self.do_pick_centerbox))
        self.machine.append(StateMachine_Item('picking_centerbox', 'ir_empty_many_times', 'verify', self.do_be_outside))
        self.machine.append(StateMachine_Item('droping_cell', 'auto', 'picking_centerbox', self.do_pick_centerbox))
        self.machine.append(StateMachine_Item('verify', 'white', 'axis_x', self.do_moveback_pick_centerbox))
        self.machine.append(StateMachine_Item('verify', 'black', 'parking', self.do_park))
        # for withdraw
        self.machine.append(StateMachine_Item('parking', 'withdraw', 'picking_cell', self.do_pickup))
        self.machine.append(StateMachine_Item('picking_cell', 'ir_check_empty', 'picking_cell', self.do_ir_check, repeat_serve=True))
        self.machine.append(StateMachine_Item('picking_cell', 'ir_check_blocked', 'droping_centerbox', self.do_pickup))
        self.machine.append(StateMachine_Item('droping_centerbox', 'end_withdraw', 'be_outside', self.do_cancel))
        self.machine.append(StateMachine_Item('be_outside', 'auto', 'parking', self.do_cancel))

        #init, to enter 'parking'
        self.current_state = 'parking'
        self.do_park(True)
        self.current_housecell = None
        self.ir_empty_count = 0

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
            if item.current_state == self.current_state:
                if item.command == command:
                    item.print_out()
                    item.service_function(house_cell)
                    self.current_state = item.post_state
                    return True
        print("TeethWarehouseRobot::update_state() failed")
        return False 

    def get_xy(self, house_cell):
        row, col = house_cell['row'], house_cell['col']
        x = row * 40
        y = col * 40
        return x,y

    def do_pick_centerbox(self, house_cell):
        self.gcode_sender.move_z_top()
        x,y = self.get_xy(house_cell)
        self.gcode_sender.move_a_to(house_cell)
        self.gcode_sender.EefMove_Z_toMiddle(12)
        self.gcode_sender.enable_vacuum_sucker(True)
        self.gcode_sender.move_z_top()

    def do_drop_denterbox(self, house_cell):
        self.gcode_sender.MoveArmToCenter()
        self.gcode_sender.enable_vacuum_sucker(False)  # drop the tooth to out-box
        self.gcode_sender.PauseForWaiting(3)
        self.gcode_sender.enable_vacuum_sucker(False)        
    
    def do_drop_cell(self, house_cell):
        pass

    def do_position_x_for_cell(self, house_cell):
        self.current_housecell = house_cell
        x = house_cell['row'] * 40
        self.gcode_sender.move_x_to(x)
    
    def do_nothing(self, house_cell):
        pass

    def do_park(self, home_park: bool):
        self.gcode_sender.Eef_TurnOn_VacuumFan(False)
        if home_park:
            self.gcode_sender.home_x()
            self.gcode_sender.home_alpha()
        else:
            self.gcode_sender.move_x_to (200)
            self.gcode_sender.move_a_to(0)

    def do_be_outside(self, house_cell):
        pass

    
    def do_feed_centerbox(self, house_cell):
        self.gcode_sender.move_a_to(0)
        self.gcode_sender.move_x_to(1200)
    
    def do_start_deposit(self, house_cell):
        for i in range(20):
            self.gcode_sender.pickup_from_centerbox()
            # self.do_ir_check()
            self.gcode_sender.drop_to_cellbox(house_cell['row'], house_cell['col'])
    
    def do_pickup(self, house_cell):
        '''
        pickup from cell   vs  pickup from centerbox ??
        '''
        self.ir_state.local_value = self.ir_state.default_value

    def do_cancel(self, house_cell):
        pass

    def do_ir_check(self, house_cell):
        match self.ir_state.remote_value:
            case 'unknown':
                # Have not get new state
                return

            case "empty":
                self.statemachine_drive('ir_check_empty', self.current_housecell)
                self.ir_state.local_value = self.ir_state.default_value
                self.ir_state.remote_value = self.ir_state.default_value
                self.ir_empty_count += 1

            case "blocked":
                self.statemachine_drive('ir_check_blocked', self.current_housecell)
                self.ir_state.local_value = self.ir_state.default_value
                self.ir_state.remote_value = self.ir_state.default_value
                self.ir_empty_count = 0

            case others:
                print('TeethWarehouseRobot::do_ir_check()', self.ir_state.remote_value)

    # def do_deposite_user_ready(self, house_cell):
    #     self.cerebellum.Dropto_CenterBox()
    #     self.cerebellum.PickupAndCheck(house_cell)

    def SpinOnce(self):
        match self.current_state:
            case 'picking_cell' | 'picking_centerbox':
                self.do_ir_check(self.house_cell)
            case 'axis_x' | 'be_outside':
                print('TeethWarehouseRobot::SpinOnce()',  'auto')
                self.statemachine_drive('auto', self.current_housecell)
            case others:
                # print('TeethWarehouseRobot::SpinOnce()'， self.current_state, self.current_housecell)
                pass

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
        # print('-------------------', request,command)
        return self.statemachine_drive(command, self.house_cell)

def test():
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
        # amq_broke_config = AMQ_BrokerConfig()
        # g_amq.connect_to_broker(amq_broke_config)
        g_amq.Subscribe("twh_221109_deposit")
        g_amq.Subscribe('twh_221109_withdraw')

        while True:
            robot.SpinOnce()
            if is_dealing_deposit:
                deal_deposit()
            else:
                deal_withdraw()

            

