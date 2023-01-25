# from von.amq_agent import g_amq, g_amq_broker_config
from wcs_robots.twh_robot_row import TwhRobot_Row
from wcs_robots.twh_robot_shipout import TwhRobot_ShipoutBox, TwhRobot_Shipout
from database.db_api import db_User,db_Stock,db_Deposite,db_Withdraw,db_Shipout
# from multiprocessing import mul
import multiprocessing
from von.mqtt_auto_sync_var import MqttAutoSyncVar
from von.mqtt_agent import g_mqtt,g_mqtt_broker_config
from von.amq_agent import g_amq, g_amq_broker_config

import time
import requests
import json


class WithdrawQueue_Tooth():
    # def __init__(self, order_id:int, shipoutbox_id:int, row:int, col:int, layer:int) -> None:
        # self.order_id = order_id
        # self.shipoutbox_id = shipoutbox_id
        # self.row =  row
        # self.col = col
        # self.layer = layer

    def __init__(self, dbtable_withdraw_queue) -> None:
        if dbtable_withdraw_queue is not None:
            self.order_id = dbtable_withdraw_queue['order_id']
            self.shipoubox_id = dbtable_withdraw_queue['connected_shipout_box']  #TODO:  unify the key's name
            self.row =  dbtable_withdraw_queue['row']
            self.col = dbtable_withdraw_queue['col']
            self.layer = dbtable_withdraw_queue['layer']


class Twh_WarehouseControlSystem():
    def __init__(self, queue_deposit:multiprocessing.Queue) -> None:
        self.robot_rows = [TwhRobot_Row(123456, 0)]
        self.robot_rows.clear()
        for i in range(4):
            new_robot_row = TwhRobot_Row(221109,i)
            self.robot_rows.append(new_robot_row)
        self.robot_shipout = TwhRobot_Shipout()
        self.withdraw_queues = []
        self.current_picking_place_tooth = WithdrawQueue_Tooth(None)
        self.current_shipping_out_box = TwhRobot_ShipoutBox(-1)
        
        self.row_robots_state = []
        self.row_robots_state.append(MqttAutoSyncVar('twh/221109/r0/state','idle'))
        self.row_robots_state.append(MqttAutoSyncVar('twh/221109/r1/state','idle'))
        self.row_robots_state.append(MqttAutoSyncVar('twh/221109/r2/state','idle'))
        self.row_robots_state.append(MqttAutoSyncVar('twh/221109/r3/state','idle'))
        
        self.button_picking_plance = MqttAutoSyncVar('twh/221109/button/pick/state','idle')
        self.button_shipout = MqttAutoSyncVar('twh/221109/button/shipout/state','idle')
        self.queue_deposit =  queue_deposit

    def FindRobotRow_idle(self) -> TwhRobot_Row:
        for robot in self.robot_rows:
            if robot.state == 'idle':
                return robot
        return None
    
    def FindTooth_from_WithdrawQueue(self, row_id: int) ->WithdrawQueue_Tooth:
        '''
        constraint:  connected_shipout_box is avaliable.
        '''
        for tooth in self.withdraw_queues:
            if tooth.shipoutbox_id != -1:
                if tooth.row == row_id:
                    return tooth
        return None

    def Assign_Shipoutbox_to_Order(self):
        '''
        Only assign one shipoutbox(the idle one) for each running.
        For the order(multi teeth) ,will move the queue from database to WCS buffer.
        '''
        # 1. find idle shipout_box
        idle_shipout_box = self.robot_shipout.FindBox_Idle()
        if idle_shipout_box is None:
            return

        # 2. get queue(same order_id) from database
        db_rows = db_Withdraw.get_single_order()
        if len(db_rows)==0:
            # the queue is empty
            return

        # 3. copy teeth in this order to wcs buffer
        # print('Twh_WarehouseControlSystem::Assign_Shipoutbox_to_Order()', db_rows)
        doc_ids = []
        for db_row in db_rows:
            doc_ids.append(db_row.doc_id)
            # new order, connect to the idle shipout_box
            tooth = WithdrawQueue_Tooth(db_row)
            tooth.shipoutbox_id = idle_shipout_box.id
            idle_shipout_box.state = 'feeding'
            self.withdraw_queues.append(tooth)
            print('Assign_Shipoutbox_to_Order() appened tooth= ', tooth)

        # 3. Delete database rows (those be copied to wcs buffer)
        db_Withdraw.table_withdraw_queue.remove(doc_ids=doc_ids)

    def Pick_and_Place(self) -> None:
        '''
        Only pick and place one tooth(from one of the idle row) for each running.
        1. find an idle row_robot.
        2. find tooth of order in the row.
        3. find connected shipout_box from the tooth
        4. start move row_robot.
        '''
        for row_robot in self.robot_rows:
            print(row_robot.state.remote_value)
            # if row_robot.state.remote_value is not None:
                # if row_robot.state.remote_value != 'ready':
                #     # print(row_robot.state.remote_value)
                #     xx= json.loads(row_robot.state.remote_value)
                #     # print(xx['is_moving'])

            if row_robot.state.remote_value == 'idle':
                tooth = self.FindTooth_from_WithdrawQueue(row_robot.id)
                if tooth is not None:
                    print('Pick_and_Place()', tooth.row, tooth.col, tooth.layer)
                    row_robot.move_to(tooth.col, tooth.layer)
                    self.withdraw_queues.remove(tooth)
                    return
            else:
                # print('Pick_and_Place()  row_robot.id   .state.remote_value  =', row_robot.id, row_robot.state.remote_value)
                pass

    def Check_MQTT_Rx(self):
        return
        for i in range(4):
            if self.row_robots_state[i].remote_value == 'ready':
                self.robot_rows[i].state = 'idle'
        
        if self.button_picking_plance.remote_value == 'pressed':
            # finished pick and place.
            self.withdraw_queues.remove(self.current_picking_place_tooth)
            if is_last_tooth_of_the_order:
                box_id = self.current_picking_place_tooth.shipoutbox_id
                box = self.robot_shipout.boxes[box_id]
                box.state = 'fullfilled'
        
        if self.button_shipout.remote_value == 'pressed':
            self.current_shipping_out_box.state = 'idle'

    def Do_deposit(self):
        if not self.queue_deposit.empty():
            new_deposit_request = self.queue_deposit.get()
            print(new_deposit_request)

def WCS_Main(queue_deposit:multiprocessing.Queue, queue_withdraw:multiprocessing.Queue):
        g_mqtt_broker_config.client_id = '20221222'
        g_mqtt.connect_to_broker(g_mqtt_broker_config)                # DebugMode, must be turn off.  
        g_amq.connect_to_broker(g_amq_broker_config)
        wcs = Twh_WarehouseControlSystem(queue_deposit)
        while True:
            # self.CheckDatabase_WithdrawQueue()
            wcs.Do_deposit()
            wcs.Assign_Shipoutbox_to_Order()
            wcs.Pick_and_Place()
            wcs.Check_MQTT_Rx()
            time.sleep(0.5)

wcs_queue_deposit = multiprocessing.Queue()
wcs_queue_withdraw = multiprocessing.Queue()
wcs_queue_takeout = multiprocessing.Queue()

def Start_WCS_Process():
    p = multiprocessing.Process(target=WCS_Main, args=(wcs_queue_deposit, wcs_queue_withdraw,))
    p.start() 
    print('WCS is running on new process.....')

    # https://pymotw.com/2/multiprocessing/communication.html#communication-between-processes




