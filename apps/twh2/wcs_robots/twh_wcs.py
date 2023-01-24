# from von.amq_agent import g_amq, g_amq_broker_config
from wcs_robots.twh_robot_row import TwhRobot_Row
from wcs_robots.twh_robot_shipout import TwhRobot_ShipoutBox, TwhRobot_Shipout
from database.db_api import g_database
from multiprocessing import Process, Value
from von.mqtt_auto_sync_var import MqttAutoSyncVar

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
            self.shipoutbox_id = dbtable_withdraw_queue['shipoutbox_id']
            self.row =  dbtable_withdraw_queue['row']
            self.col = dbtable_withdraw_queue['col']
            self.layer = dbtable_withdraw_queue['layer']


class Twh_WarehouseControlSystem():
    def __init__(self) -> None:
        self.robot_rows = [TwhRobot_Row(111222, 0)]
        self.robot_rows.clear()
        for i in range(4):
            new_robot_row = TwhRobot_Row(2222,i)
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

    def FindRobotRow_idle(self) -> TwhRobot_Row:
        for robot in self.robot_rows:
            if robot.state == 'idle':
                return robot
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

        # 2. Copy queue(same order_id) from database  to wcs buffer
        db_rows = g_database.table_withdraw_queue.search('where order_id=min(order_id)')
        doc_ids = []
        for db_row in db_rows:
            doc_ids.append(db_row.doc_id)
            # new order, connect to the idle shipout_box
            tooth = WithdrawQueue_Tooth(db_row)
            tooth.shipoutbox_id = idle_shipout_box.id
            idle_shipout_box.state = 'feeding'

        # 3. Delete database rows (those be copied to wcs buffer)
        g_database.table_withdraw_queue.remove(doc_ids=doc_ids)

    def FindTooth_from_WithdrawQueue(self, row_id: int) ->WithdrawQueue_Tooth:
        '''
        constraint:  connected_shipout_box is avaliable.
        '''
        for tooth in self.withdraw_queues:
            if tooth.shipout_box_id != -1:
                if tooth.row == row_id:
                    return tooth
        return None

    def Pick_and_Place(self) -> None:
        '''
        Only pick and place one tooth(from one of the idle row) for each running.
        1. find an idle row_robot.
        2. find tooth of order in the row.
        3. find connected shipout_box from the tooth
        4. start move row_robot.
        '''
        for row_robot in self.robot_rows:
            if not row_robot.is_moving:
                # Got an idle row_robot
                tooth = self.FindTooth_from_WithdrawQueue(row_robot.row_id)
                if tooth is not None:
                    row_robot.move_to(tooth.col, tooth.layer)
                    self.withdraw_queues.remove(tooth)
                    return

    def Check_MQTT_Rx(self):
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

    def Spin(self):
        while True:
            # self.CheckDatabase_WithdrawQueue()
            self.Assign_Shipoutbox_to_Order()
            self.Pick_and_Place()
            self.Check_MQTT_Rx()
            time.sleep(0.5)

    def StartProcess(self):
        p = Process(target=self.Spin)
        p.start() 
        print('WCS is running on new process.....')



