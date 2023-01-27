from wcs_robots.twh_robot_row import TwhRobot_Row
from wcs_robots.twh_robot_packer import TwhRobot_PackBox, TwhRobot_Packer
from wcs_robots.gcode_sender import gcode_senders_spin_once
from database.db_api import db_User, db_Stock, db_Deposite, db_Withdraw, db_Shipout
import multiprocessing
from von.remote_var_mqtt import RemoteVar_mqtt
from von.mqtt_agent import g_mqtt,g_mqtt_broker_config

import time


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
            self.packbox_id = dbtable_withdraw_queue['connected_shipout_box']  #TODO:  unify the key's name
            self.row =  dbtable_withdraw_queue['row']
            self.col = dbtable_withdraw_queue['col']
            self.layer = dbtable_withdraw_queue['layer']

    def print_out(self):

        ss = ' Tooth order_id=' + str(self.order_id)
        ss += ' packbox_id=' + str(self.packbox_id)
        ss += ' row=' + str(self.row)
        ss += ' col=' + str(self.col)
        ss += ' layer= ' + str(self.layer)
        print(ss)

class Twh_WarehouseControlSystem():

    def __init__(self, queue_deposit:multiprocessing.Queue) -> None:
        # 1. Create row_robots list
        self.robot_rows = [TwhRobot_Row('123456', 0)]
        self.robot_rows.clear()
        for i in range(4):
            new_robot_row = TwhRobot_Row('221109',i)
            self.robot_rows.append(new_robot_row)
        # 2. Create packer_robot
        self.packer = TwhRobot_Packer()
        self.packer.debug()
        # 3. Create queues
        self.withdraw_queues = []
        self.current_picking_place_tooth = WithdrawQueue_Tooth(None)
        self.current_packbox = TwhRobot_PackBox(-1)
        
        self.row_robots_state = []
        self.row_robots_state.append(RemoteVar_mqtt('twh/221109/r0/state', 'idle'))
        self.row_robots_state.append(RemoteVar_mqtt('twh/221109/r1/state', 'idle'))
        self.row_robots_state.append(RemoteVar_mqtt('twh/221109/r2/state', 'idle'))
        self.row_robots_state.append(RemoteVar_mqtt('twh/221109/r3/state','idle'))
        
        self.button_pick = RemoteVar_mqtt('twh/221109/button_pick/state','idle')
        self.button_pack = RemoteVar_mqtt('twh/221109/button_pack/state','idle')
        self.queue_deposit =  queue_deposit

    def FindRobotRow_idle(self) -> TwhRobot_Row:
        for robot in self.robot_rows:
            if robot.state == 'idle':
                return robot
        return None
    
    def FindTooth_from_WithdrawQueue(self, row_id: int) ->WithdrawQueue_Tooth:
        '''
        constraint:  connected_pack_box is avaliable.
        '''
        for tooth in self.withdraw_queues:
            if tooth.packbox_id != -1:
                if tooth.row == row_id:
                    return tooth
        return None

    def Assign_Packbox_to_Order(self):
        '''
        Only assign one shipoutbox(the idle one) for each running.
        For the order(multi teeth) ,will move the queue from database to WCS buffer.
        '''
        # 1. find idle shipout_box
        idle_packbox = self.packer.FindBox_Idle()
        if idle_packbox is None:
            return

        # 2. get queue(same order_id) from database
        order_items = db_Withdraw.get_single_order()
        if len(order_items)==0:
            # the queue is empty
            return

        # 3. copy teeth in this order to wcs buffer
        # print('Twh_WarehouseControlSystem::Assign_Shipoutbox_to_Order()', db_rows)
        doc_ids = []
        for order_item in order_items:
            # print('ddddddddddddddddd', db_row)
            doc_ids.append(order_item.doc_id)
            # new order, connect to the idle shipout_box
            new_tooth = WithdrawQueue_Tooth(order_item)
            self.withdraw_queues.append(new_tooth)
            new_tooth.shipout_box_id = idle_packbox.id
            idle_packbox.state = 'feeding'

            # idle_shipout_box.print_out()
            # for t in self.withdraw_queues:
            #     t.print_out()

        # 3. Delete database rows (those be copied to wcs buffer)
        db_Withdraw.table_withdraw_queue.remove(doc_ids=doc_ids)

    def Move_Pick_Pack(self) -> None:
        '''
        Only pick and place one tooth(from one of the idle row) for each running.
        1. find an idle row_robot.
        2. find tooth of order in the row.
        3. find connected shipout_box from the tooth
        4. start move row_robot.
        '''
        for row_robot in self.robot_rows:
            # print(row_robot.state.remote_value)
            # if row_robot.state.remote_value is not None:
                # if row_robot.state.remote_value != 'ready':
                #     # print(row_robot.state.remote_value)
                #     xx= json.loads(row_robot.state.remote_value)
                #     # print(xx['is_moving'])

            if row_robot.state.get() in ['idle', 'ready']:
                tooth = self.FindTooth_from_WithdrawQueue(row_robot.id)
                if tooth is not None:
                    print('Move_Pick_Pack()', tooth.row, tooth.col, tooth.layer)
                    row_robot.move_to(tooth.col, tooth.layer, tooth.packbox_id)
                    self.withdraw_queues.remove(tooth)
                    row_robot.state.set('moving')
                    return
            elif row_robot.state.get() == 'ready':
                # show green led on row_robot 
                row_robot.show_layer_led()
                # show green led on pack_box
                self.packer.show_pack_box_led_packing(row_robot.connected_packing_box)
                # wait operator press the button
                self.button_pick.set('unpressed')
                self.state = 'picking_packing'
                
            if self.state == 'picking_packing':
                if self.button_pick.get() == 'pressed':
                    # turn off all green leds
                    self.state = 'idle'
                    row_robot.state.set('idle')
            else:
                # print('Move_Pick_Pack()   id, state == ', row_robot.id, row_robot.state.get() )
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
                box = self.packer.boxes[box_id]
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
        wcs = Twh_WarehouseControlSystem(queue_deposit)
        while True:
            # self.CheckDatabase_WithdrawQueue()
            wcs.Do_deposit()
            wcs.Assign_Packbox_to_Order()
            wcs.Move_Pick_Pack()
            wcs.Check_MQTT_Rx()
            gcode_senders_spin_once()
            time.sleep(0.5)

wcs_queue_deposit = multiprocessing.Queue()
wcs_queue_withdraw = multiprocessing.Queue()
wcs_queue_takeout = multiprocessing.Queue()

def Start_WCS_Process():
    p = multiprocessing.Process(target=WCS_Main, args=(wcs_queue_deposit, wcs_queue_withdraw,))
    p.start() 
    print('WCS is running on new process.....')

    # https://pymotw.com/2/multiprocessing/communication.html#communication-between-processes




