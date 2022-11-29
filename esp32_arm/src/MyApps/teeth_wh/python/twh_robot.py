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
            self.reset()
        else:
            obj = json.loads(json_string)
            self.robot_id = obj['dev']
            self.command = obj['command']
            self.row = obj['cell']['row']
            self.col = obj['cell']['col']
        
    def reset(self):
        self.robot_id = 0
        self.command = ''
        self.row = 0
        self.col = 0


class TeethWarehouseRobot():
    
    def __init__(self, robot_id) -> None:
        self.robot_id = robot_id
        self.withdraw_request = UserRequest('')
        self.deposit_request = UserRequest('')

        self.ir_state = MqttAutoSyncVar("twh/" + str(robot_id) + "/ir_state", "unknown")
        self.ir_state.auto_sync_to_local = False
        self.ir_state.auto_sync_to_remote = True
        self.gcode_sender = GcodeSender()
        self.__init_eef_statemachine()
        # self.deposit_queue_name = 'twh_'+str(robot_id)+'_deposit'
        # self.withdraw_queue_name = 'twh_'+str(robot_id)+'_withdraw'
        self.deposit_queue_name = 'twh_deposit'
        self.withdraw_queue_name = 'twh_withdraw'
        g_amq.Subscribe(queue_name = self.deposit_queue_name)
        g_amq.Subscribe(queue_name= self.withdraw_queue_name)
    
    def spin_once(self):
        # print("current state=", self.eef_statemachine.current_state)
        is_accepted = self.eef_statemachine.stimulate(command=self.deposit_request.command, row=self.deposit_request.row, col=self.deposit_request.col)
        if is_accepted:
            self.deposit_request.reset()
            return

        is_accepted = self.eef_statemachine.stimulate(command=self.withdraw_request.command, row=self.withdraw_request.row, col=self.withdraw_request.col)
        if is_accepted:
            self.withdraw_request.reset()
            return

        match self.eef_statemachine.current_state:
            case 'picking_cell':
                self.eef_do_ir_check(row=self.withdraw_request.row, col=self.withdraw_request.col)
            case 'be_outside':
                print('TeethWarehouseRobot::SpinOnce()',  'auto')
                self.eef_statemachine.stimulate('auto', row=self.withdraw_request.row, col=self.withdraw_request.col)                
            case 'picking_centerbox':
                self.eef_do_ir_check(row=self.deposit_request.row, col=self.deposit_request.col)
            case 'axis_x':
                self.eef_statemachine.stimulate('auto', row=self.deposit_request.row, col=self.deposit_request.col)

        if self.withdraw_request.command == '':
            payload = g_amq.fetch_message(self.withdraw_queue_name)
            if payload is not None:
                request_string = payload.decode('utf-8')
                print('withdraw request ', payload)
                self.withdraw_request = UserRequest(request_string)

        if self.deposit_request.command == '':
            # print('deposit request is blank.   ', self.deposit_queue_name)
            payload = g_amq.fetch_message(self.deposit_queue_name)
            if payload is not None:
                request_string = payload.decode('utf-8')
                print('deposit request---- ', payload)
                self.deposit_request = UserRequest(request_string)


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
        self.eef_statemachine = StateMachine('robot_eef', 'parking')
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
        self.eef_statemachine.AppendItem(StateMachine_Item('picking_cell', 'ir_check_empty', 'picking_cell', self.eef_do_pickup))
        self.eef_statemachine.AppendItem(StateMachine_Item('picking_cell', 'ir_check_blocked', 'droping_centerbox', self.eef_do_drop_denterbox))
        self.eef_statemachine.AppendItem(StateMachine_Item('droping_centerbox', 'withdraw', 'picking_cell', self.eef_do_pickup))
        self.eef_statemachine.AppendItem(StateMachine_Item('droping_centerbox', 'end_withdraw', 'be_outside', self.eef_do_be_outside))
        self.eef_statemachine.AppendItem(StateMachine_Item('be_outside', 'auto', 'parking', self.eef_do_park))

        self.eef_do_park(True)
        self.ir_empty_count = 0


    def get_xy(self, row,col):
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

    def eef_do_pick_centerbox(self, row,col):
        self.gcode_sender.move_z_top()
        x,y = self.get_xy(row,col)
        self.gcode_sender.move_xy_to(x,y)
        self.gcode_sender.EefMove_Z_toMiddle(12)
        self.gcode_sender.enable_vacuum_sucker(True)
        self.gcode_sender.move_z_top()

    def eef_do_drop_denterbox(self, row, col):
        self.gcode_sender.MoveArmToCenter()
        self.gcode_sender.enable_vacuum_sucker(False)  # drop the tooth to out-box
        self.gcode_sender.PauseForWaiting(3)
        self.gcode_sender.enable_vacuum_sucker(False)        
    
    def eef_do_drop_cell(self, row, col):
        pass

    def eef_do_position_x_for_cell(self, row, col):
        x = row * 40
        y = col * 40
        self.gcode_sender.move_xy_to(x,y)
    
    def do_nothing(self, row, col):
        pass

    def eef_do_park(self, home_park: bool):
        self.gcode_sender.Eef_TurnOn_VacuumFan(False)
        if home_park:
            self.gcode_sender.home_x()
            self.gcode_sender.home_alpha()
        else:
            self.gcode_sender.move_x_to (200)
            self.gcode_sender.move_a_to(0)

    def eef_do_be_outside(self, row, col):
        pass

    def eef_do_moveback_pick_centerbox(self, row, col):
        pass

    def eef_do_feed_centerbox(self, row, col):
        self.gcode_sender.move_a_to(0)
        self.gcode_sender.move_xy_to(1000, 1)
    
    def eef_do_start_deposit(self, row, col):
        for i in range(20):
            self.gcode_sender.pickup_from_centerbox()
            # self.do_ir_check()
            self.gcode_sender.drop_to_cellbox(house_cell['row'], house_cell['col'])
    
    def eef_do_pickup(self, row, col):
        '''
        pickup from cell   vs  pickup from centerbox ??
        '''
        self.ir_state.local_value = self.ir_state.default_value

    def eef_do_cancel(self, row, col):
        pass

    def eef_do_ir_check(self, row, col):
        match self.ir_state.remote_value:
            case 'unknown':
                # Have not get new state
                return

            case "empty":
                self.eef_statemachine.stimulate('ir_check_empty', row, col)
                self.ir_state.local_value = self.ir_state.default_value
                self.ir_state.remote_value = self.ir_state.default_value
                self.ir_empty_count += 1

            case "blocked":
                self.eef_statemachine.stimulate('ir_check_blocked', row, col)
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
    while True:
        g_amq.SpinOnce()
        twh_message_dealler.spin_once()
