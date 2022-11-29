from von.rabbitmq_agent import g_amq, AMQ_BrokerConfig
from von.mqtt_auto_syncer import MqttAutoSyncVar
from von.mqtt_agent import g_mqtt, g_mqtt_broker_config
from gcode_sender import GcodeSender
from statemachine import StateMachine_Item,StateMachine
import json
# How to Convert JSON Data Into a Python Object?   https://www.youtube.com/watch?v=hJ2HfejqppE 

class UserRequest:

    def __init__(self, json_string: str) -> None:
        if json_string == '':
            self.robot_id = 0
            self.command = ''
            self.row = 0
            self.col = 0
        else:
            obj = json.loads(json_string)
            self.robot_id = obj['dev']
            self.command = obj['command']
            self.row = obj['cell']['row']
            self.col = obj['cell']['col']


class TeethWarehouseRobot():
    
    def __init__(self, robot_id) -> None:
        self.robot_id = robot_id
        self.request_withdraw = UserRequest('')
        self.request_deposit = UserRequest('')
        self.direction = self.__get_direction(self.request_withdraw)
        self.eef_statemachine = StateMachine()
             

        self.ir_state = MqttAutoSyncVar("twh/" + str(robot_id) + "/ir_state", "unknown")
        self.ir_state.auto_sync_to_local = False
        self.ir_state.auto_sync_to_remote = True
        self.gcode_sender = GcodeSender()
        self.__init_eef_statemachine()
        # self.deposit_queue_name = 'twh_'+str(robot_id)+'_deposit'
        # self.withdraw_queue_name = 'twh_'+str(robot_id)+'_withdraw'
        self.deposit_queue_name = 'twh_deposit'
        self.withdraw_queue_name = 'twh_withdraw'
        self.current_queue_name = self.deposit_queue_name
        g_amq.Subscribe(queue_name = self.deposit_queue_name)
        g_amq.Subscribe(queue_name= self.withdraw_queue_name)
    
    def __get_direction(self, user_request: UserRequest):
        if user_request is None:
            self.direction = 'None'
            return
        if user_request.command in ['prepare_deposit','start_deposit']:
            self.direction = 'deposit'
        if user_request.command in ['withdraw', 'end_withdraw']:
            self.direction = 'withdraw'
        
    def spin_once(self):
        match self.current_state:
            case 'picking_cell' | 'picking_centerbox':
                self.do_ir_check(self.house_cell)
            case 'axis_x' | 'be_outside':
                print('TeethWarehouseRobot::SpinOnce()',  'auto')
                self.eef_statemachine_drive('auto', self.current_housecell)
            case others:
                # print('TeethWarehouseRobot::SpinOnce()'， self.current_state, self.current_housecell)
                pass

        if self.request_withdraw.command != '':
            print ('Trying to feed a request.', self.request_withdraw)
            self.feed_request(self.request_withdraw)
            return
        
        payload = g_amq.fetch_message(self.current_queue_name)
        if payload is not None:
            request_string = payload.decode('utf-8')
            self.request_withdraw = UserRequest(request_string)
            return

        # current queue is empty, consider to change between deposit and withdraw
        if self.current_queue_name == self.deposit_queue_name:
            self.current_queue_name = self.withdraw_queue_name
        else:
            self.current_queue_name = self.deposit_queue_name
        # self.request_withdraw = UserRequest(g_amq.fetch_message(self.current_queue_name))

    def feed_request(self, user_request: UserRequest) -> bool:
        '''
        This should start a new thread.
        Reason: Dynamic gcode follow a  branched processing flow. Synced with hardware robot. 
                And take a long time up to couple minutes.
        Return:
            True: The command is accepted by state machine
            False: The commnd is rejected by state machine
        '''

        # self.house_cell = request['cell']
        # if request.Acceptable():
        # print('-------------------', request,command)
        return self.eef_statemachine_drive(user_request.command, user_request.get_cell())

    def __init_mq_statemachine(self):
        self.mq_statemachine = StateMachine('User Request Queue')
        self.mq_statemachine.AppendItem(StateMachine_Item('idle', 'withdraw', 'withdraw', self.mq_do_fetch_withdraw_enter))
        self.mq_statemachine.AppendItem (StateMachine_Item('withdraw', 'withdraw', 'withdraw', self.do_fetch_withdraw_keep))
        self.mq_statemachine.AppendItem (StateMachine_Item('withdraw', 'idle', 'idle', self.do_nothing_exit))
        self.mq_statemachine.AppendItem (StateMachine_Item('idle', 'deposit', 'deposit', self.do_nothing))
        self.mq_statemachine.AppendItem (StateMachine_Item('deposit', 'deposit', 'deposit', self.do_nothing))
        self.mq_statemachine.AppendItem (StateMachine_Item('deposit', 'idle', 'idle', self.do_nothing))


    def __init_eef_statemachine(self):
        # for deposit
        self.eef_statemachine.AppendItem(StateMachine_Item('parking', 'prepare_deposit', 'feeding_centerbox', self.eef_do_feed_centerbox))
        self.eef_statemachine.AppendItem(StateMachine_Item('feeding_centerbox', 'end_deposit', 'parking', self.eef_do_park))
        self.eef_statemachine.AppendItem(StateMachine_Item('feeding_centerbox', 'prepare_deposit', 'feeding_centerbox', self.do_nothing))
        self.eef_statemachine.AppendItem(StateMachine_Item('feeding_centerbox', 'start_deposit', 'axis_x', self.eef_do_position_x_for_cell))
        self.eef_statemachine.AppendItem(StateMachine_Item('axis_x', 'auto', 'picking_centerbox', self.eef_do_pick_centerbox))
        self.eef_statemachine.AppendItem(StateMachine_Item('picking_centerbox', 'ir_check_blocked', 'droping_cell', self.eef_do_drop_cell))
        self.eef_statemachine.AppendItem(StateMachine_Item('picking_centerbox', 'ir_check_empty', 'picking_centerbox', self.eef_do_pick_centerbox))
        self.eef_statemachine.AppendItem(StateMachine_Item('picking_centerbox', 'ir_empty_many_times', 'verify', self.eef_do_be_outside))
        self.eef_statemachine.AppendItem(StateMachine_Item('droping_cell', 'auto', 'picking_centerbox', self.eef_do_pick_centerbox))
        self.eef_statemachine.AppendItem(StateMachine_Item('verify', 'white', 'axis_x', self.eef_do_moveback_pick_centerbox))
        self.eef_statemachine.AppendItem(StateMachine_Item('verify', 'black', 'parking', self.eef_do_park))
        # for withdraw
        self.eef_statemachine.AppendItem(StateMachine_Item('parking', 'withdraw', 'picking_cell', self.eef_do_pickup))
        self.eef_statemachine.AppendItem(StateMachine_Item('picking_cell', 'ir_check_empty', 'picking_cell', self.eef_do_ir_check, repeat_serve=True))
        self.eef_statemachine.AppendItem(StateMachine_Item('picking_cell', 'ir_check_blocked', 'droping_centerbox', self.eef_do_pickup))
        self.eef_statemachine.AppendItem(StateMachine_Item('droping_centerbox', 'end_withdraw', 'be_outside', self.eef_do_cancel))
        self.eef_statemachine.AppendItem(StateMachine_Item('be_outside', 'auto', 'parking', self.eef_do_cancel))

        #init, to enter 'parking'
        self.current_state = 'parking'
        self.eef_do_park(True)
        # self.current_housecell = None
        self.ir_empty_count = 0





    def get_xy(self, house_cell):
        row, col = house_cell['row'], house_cell['col']
        x = row * 40
        y = col * 40
        return x,y

    def mq_do_fetch_deposit_enter(self):
        pass

    def mq_do_fetch_withdraw_enter(self):
        pass
    def mq_do_fetch_withdraw_keep(self):
        pass
    def mq_do_fetch_withdraw_exit(self):
        pass

    def eef_do_pick_centerbox(self, house_cell):
        self.gcode_sender.move_z_top()
        x,y = self.get_xy(house_cell)
        self.gcode_sender.move_a_to(house_cell)
        self.gcode_sender.EefMove_Z_toMiddle(12)
        self.gcode_sender.enable_vacuum_sucker(True)
        self.gcode_sender.move_z_top()

    def eef_do_drop_denterbox(self, house_cell):
        self.gcode_sender.MoveArmToCenter()
        self.gcode_sender.enable_vacuum_sucker(False)  # drop the tooth to out-box
        self.gcode_sender.PauseForWaiting(3)
        self.gcode_sender.enable_vacuum_sucker(False)        
    
    def eef_do_drop_cell(self, house_cell):
        pass

    def eef_do_position_x_for_cell(self, house_cell):
        self.current_housecell = house_cell
        x = house_cell['row'] * 40
        self.gcode_sender.move_x_to(x)
    
    def do_nothing(self, house_cell):
        pass

    def eef_do_park(self, home_park: bool):
        self.gcode_sender.Eef_TurnOn_VacuumFan(False)
        if home_park:
            self.gcode_sender.home_x()
            self.gcode_sender.home_alpha()
        else:
            self.gcode_sender.move_x_to (200)
            self.gcode_sender.move_a_to(0)

    def eef_do_be_outside(self, house_cell):
        pass

    def eef_do_moveback_pick_centerbox(self, house_cell):
        pass

    def eef_do_feed_centerbox(self, house_cell):
        self.gcode_sender.move_a_to(0)
        self.gcode_sender.move_xy_to(1000, 1)
    
    def eef_do_start_deposit(self, house_cell):
        for i in range(20):
            self.gcode_sender.pickup_from_centerbox()
            # self.do_ir_check()
            self.gcode_sender.drop_to_cellbox(house_cell['row'], house_cell['col'])
    
    def eef_do_pickup(self, house_cell):
        '''
        pickup from cell   vs  pickup from centerbox ??
        '''
        self.ir_state.local_value = self.ir_state.default_value

    def eef_do_cancel(self, house_cell):
        pass

    def eef_do_ir_check(self, house_cell):
        match self.ir_state.remote_value:
            case 'unknown':
                # Have not get new state
                return

            case "empty":
                self.eef_statemachine_drive('ir_check_empty', self.current_housecell)
                self.ir_state.local_value = self.ir_state.default_value
                self.ir_state.remote_value = self.ir_state.default_value
                self.ir_empty_count += 1

            case "blocked":
                self.eef_statemachine_drive('ir_check_blocked', self.current_housecell)
                self.ir_state.local_value = self.ir_state.default_value
                self.ir_state.remote_value = self.ir_state.default_value
                self.ir_empty_count = 0

            case others:
                print('TeethWarehouseRobot::do_ir_check()', self.ir_state.remote_value)

    # def do_deposite_user_ready(self, house_cell):
    #     self.cerebellum.Dropto_CenterBox()
    #     self.cerebellum.PickupAndCheck(house_cell)



class TeethWarehouseMessageDispacher():
    global g_amq

    def __init__(self) -> None:
        self.robots = [TeethWarehouseRobot(221190)]

        # g_amq.Subscribe(queue_name='twh_deposit')
        # g_amq.Subscribe(queue_name='twh_withdraw')

    def __find_robot(self, robot_id:int) -> TeethWarehouseRobot:
        for r in self.robots:
            if r.robot_id == robot_id:
                return r
        return None

    def __deal_deposit(self, user_request: UserRequest) -> bool:
        '''
        return True:  The user_request message is consumed.
        return False: The user_request message is blocked.
        '''
        robot = self.find_robot(user_request.robot_id)
        if robot is None:
            new_robot = TeethWarehouseRobot(user_request.robot_id)
            self.robots.append(new_robot)
            robot = new_robot
  
        is_fed =robot.feed_request(user_request.command)

    def spin_once(self):
        for r in self.robots:
            r.spin_once()
            



if __name__ == '__main__':
    amq_broke_config = AMQ_BrokerConfig()
    g_amq.connect_to_broker(amq_broke_config)
    g_mqtt.connect_to_broker(g_mqtt_broker_config)
    twh_message_dealler = TeethWarehouseMessageDispacher()
    g_amq.SpinOnce()
    while True:
        twh_message_dealler.spin_once()
