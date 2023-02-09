from statemachine import StateMachine_Item, StateMachine
from von.amq_agent import g_amq, g_amq_broker_config
from twh_robot_layer import TwhRobot_Layer
import requests
import json

class TwhRobot_v2():
    def __init__(self) -> None:
        g_amq.Subscribe('twh_deposit')
        g_amq.Subscribe('twh_withdraw')
        self.layer_robots = [TwhRobot_Layer(robot_id='221109')]
        self.current_state = 'idle'
        # self.state_mathine = StateMachine(machine_name='TwhRobot', init_state='idle')
        # self.state_mathine.AppendItem(StateMachine_Item('idle','check_deposit_mq','checking_deposit_mq', self.sm_check_deposit_mq))        

    def state_machine_spin_once(self):
        
        match self.current_state:
            case 'idle':
                self.current_state = 'checking_deposit_mq'

            case 'checking_deposit_mq':
                self.sm_check_deposit_mq()

            case 'checking_withdraw_mq':
                self.sm_check_withdraw_mq()

            case 'deposit':
                self.sm_do_deposit()

            case 'withdraw':
                self.sm_do_withdraw()

            case _:
                print("TwhRobot::state_machine_spin_once()", self.current_state)

    def sm_check_deposit_mq(self):
        msg = g_amq.fetch_message_payload('twh_deposit')
        if msg is None:
            self.current_state = 'checking_withdraw_mq'
        else:
            # let robot moving
            msg_str = msg.decode('utf-8').replace("'", '"')
            # print("sm_check_deposit_mq", msg_str)
            user_request = json.loads(msg_str)
            # print("sm_check_deposit_mq", user_request)
            row = int(user_request['row'])
            col = int(user_request['col'])
            # print("sm_check_deposit_mq", row, col)
            self.layer_robots[0].move_to(row,col)
            self.current_state = 'deposit'

    def sm_do_deposit(self):
        # if user_finished, update stock?\
        self.current_state = 'idle'

    def sm_check_withdraw_mq(self):
        msg = g_amq.fetch_message_payload('twh_withdraw')
        if msg is None:
            self.current_state = 'idle'
        else:
            # print('TwhRobot::sm_check_withdraw_mq()',msg)
            msg_str = (msg.decode('utf-8').replace("'", '"'))
            # print('TwhRobot::sm_check_withdraw_mq()',msg_str)
            self.withdraw_list = json.loads(msg_str)
            self.current_state = 'withdraw'

    def decrease_stock(self, doc_id:int):
        url = 'http://localhost:5000/decrease_stock?doc_id=' + str(doc_id)
        response = requests.get(url)
        print(response)

    def sm_do_withdraw(self):
        if len(self.withdraw_list) == 0:
            self.current_state = 'idle'

        for layer in range(0,1):
            for row in range(0,3):
                layer_robot = self.layer_robots[layer]
                row_robot = self.layer_robots[layer].row_robots[row]
                if layer_robot.current_state == 'dropped':
                    done_item = row_robot.get_withdraw_item()
                    # update database
                    self.decrease_stock(done_item['doc_id'])
                    # remove member in list
                    self.withdraw_list.remove()
                    layer_robot.reset_current_state_to_idle()  # will reset row_robot inside.
            
        for box in self.withdraw_list:
            # print('@@@@@@')
            # print(box)
            layer = int(box['layer'])
            row = int(box['row'])
            col = int(box['col'])
            layer_robot = self.layer_robots[layer]
            row_robot = self.layer_robots[layer].row_robots[row]
            if row_robot.current_state == 'idle':
                row_robot.move_to(col)


if __name__ == '__main__':
    from von.mqtt_agent import g_mqtt, g_mqtt_broker_config
    g_mqtt_broker_config.client_id = '221223'
    g_mqtt.connect_to_broker(g_mqtt_broker_config)
    g_amq.connect_to_broker(g_amq_broker_config)


    robot = TwhRobot()
    while True:
        g_amq.process_data_events()
        robot.state_machine_spin_once()