from wcs_robots.twh_robot_loop_porter import TwhRobot_LoopPorter
from wcs_robots.twh_robot_packer import TwhRobot_Packer
from wcs_robots.gcode_sender import gcode_senders_spin_once
from bolt_nut import PickingPacking_Tooth
# from database.db_api import db_User, db_Stock, db_Deposite, db_Withdraw, db_Shipout
from web_stock.db_api import db_Shipout,db_Stock,db_Deposite,db_Withdraw
from web_user.db_api import db_User

import multiprocessing
from von.remote_var_mqtt import RemoteVar_mqtt
from von.mqtt_agent import g_mqtt,g_mqtt_broker_config
import time
from logger import Logger


class Twh_WarehouseControlSystem():

    def __init__(self, queue_deposit:multiprocessing.Queue) -> None:
        self.state = 'idle'
        # 1. Create row_robots list
        self.porters = []
        for i in range(4):
            new_porter = TwhRobot_LoopPorter('221109', i)
            self.porters.append(new_porter)
        # 2. Create packer_robot
        self.packer = TwhRobot_Packer()
        # 3. Create queues
        # When there is a request for picking,  put the tooth into the queue.
        # when chain loop start moving, the teeth will be removed.
        self.withdraw_request_queue = []  

        self.current_picking_packing_porter = None    
        self.button_pick = RemoteVar_mqtt('twh/221109/button_pick/state','idle')
        self.button_pack = RemoteVar_mqtt('twh/221109/button_pack/state','idle')

        self.queue_deposit =  queue_deposit

    def FindPorter_idle(self) -> TwhRobot_LoopPorter:
        for robot in self.porters:
            # Logger.Print('FindPorter_idle()   robot.state=', robot.state.get())
            if robot.state.get() == 'idle':
                return robot
        return None
    
    # def _withdraw_teeth_queue_get_portable (self, row_id: int) ->PickingPacking_Tooth:
    #     '''
    #     constraint:  connected_pack_box is avaliable.
    #     '''
    #     for tooth in self.withdraw_request_queue:
    #         if tooth.packbox_id != -1:
    #             if tooth.row == row_id:
    #                 return tooth
    #     return None

    def Assign_Packbox_to_Order(self):
        '''
        Only assign one shipoutbox(the idle one) for each running.
        For the order(multi teeth) ,will move the queue from database to WCS buffer.
        '''
        # 1. find idle shipout_box
        idle_packbox = self.packer.FindBox_Idle()
        if idle_packbox is None:
            return
        # Logger.Debug('Twh_WarehouseControlSystem::Assign_Packbox_to_Order()   Found idle_packbox')
        # Logger.Print('box_id', idle_packbox.id)

        # 2. get queue(same order_id) from database
        order_items = db_Withdraw.get_single_order()
        if len(order_items)==0:
            # the queue is empty
            # Logger.Info('Twh_WarehouseControlSystem::Assign_Packbox_to_Order()   Withdraw queue is empty.')
            return

        # 3. copy teeth in this order to wcs buffer
        Logger.Debug('Twh_WarehouseControlSystem::Assign_Shipoutbox_to_Order()  Start copy.')
        doc_ids = []
        for order_item in order_items:
            # Logger.Print('Assign_Packbox_to_Order()  \n  ', order_item)
            doc_ids.append(order_item.doc_id)
            # new order, connect to the idle shipout_box
            new_tooth = PickingPacking_Tooth(order_item)
            new_tooth.packbox_id = idle_packbox.id
            new_tooth.order_id = order_item['order_id']
            new_tooth.print_out('Twh_WarehouseControlSystem::Assign_Shipoutbox_to_Order()   New tooth in picking_queue')

            self.withdraw_request_queue.append(new_tooth)
            idle_packbox.state = 'feeding'   #??????   any string exclue "idle", right?

            # self.packer.PrintOut('Twh_WarehouseControlSystem::Assign_Shipoutbox_to_Order()  view packer')


        # 4. Delete teeth in database (those be copied to wcs buffer)
        db_Withdraw.table_withdraw_queue.remove(doc_ids=doc_ids)

    def _withdraw_teeth_queue_get_portable(self) -> PickingPacking_Tooth:
        for target_tooth in self.withdraw_request_queue:
            # constraint:  connected_pack_box is avaliable.
            if target_tooth.packbox_id != -1:
                if (self.porters[target_tooth.row].state.get() == 'idle'):
                    return target_tooth
        return None
                
  




    def Pick_Pack_Port_Pair(self) -> None:
        '''
        Only pick and place one tooth(from one of the idle row) for each running.
        1. find an idle porter.
        2. find tooth of order in the row.
        3. find connected shipout_box from the tooth
        4. start move porter.
        '''
        # Logger.Debug("Twh_WarehouseControlSystem::twh_Move_Pick_Pack()")
        if self.state == 'picking_packing':
            if self.button_pick.get() == 'pressed':
                # turn off all green leds
                Logger.Print('Twh_WarehouseControlSystem::twh_Move_Pick_Pack()  button_is_pressed','turn_to_idle')
                self.state = 'idle'
                self.current_picking_packing_porter.state.set('idle')
                return
        
        # if any porter is ready for picking_packing, do it.
        for porter in self.porters:
            # Logger.Print('Twh_WarehouseControlSystem::twh_Move_Pick_Pack()   porter.state.get() ', porter.state.get())
            if porter.state.get() == 'ready':
                # show green led on porter, and on packer
                Logger.Print('Twh_WarehouseControlSystem::twh_Move_Pick_Pack()  Porter is ready. porter_id', porter.id)
                porter.state.set('picking_packing')
                porter.show_layer_led()
                self.packer.show_pack_box_led(porter.target_tooth.packbox_id)
                # wait operator to press the button
                self.button_pick.set('unpressed')
                self.current_picking_packing_porter = porter
                self.state = 'picking_packing'
                return

        portable_tooth =  self._withdraw_teeth_queue_get_portable()
        if portable_tooth is None:
            return

        portable_tooth.print_out("Portable idle_porter and portable_tooth ")
        idle_porter = self.porters[portable_tooth.row]
        idle_porter.port_to_pick(portable_tooth)
        print('Twh_WarehouseControlSystem::twh_Move_Pick_Pack()  Found target_tooth ', portable_tooth.row, portable_tooth.col, portable_tooth.layer)
        self.withdraw_request_queue.remove(portable_tooth)   # If move this line after packing,  will be takeout from the queue repeatly?


    def Do_deposit(self):
        if not self.queue_deposit.empty():
            new_deposit_request = self.queue_deposit.get()
            print(new_deposit_request)

def WCS_Main(queue_deposit:multiprocessing.Queue):
        g_mqtt_broker_config.client_id = '20221222'
        g_mqtt.connect_to_broker(g_mqtt_broker_config)                # DebugMode, must be turn off.  
        wcs = Twh_WarehouseControlSystem(queue_deposit)
        while True:
            # self.CheckDatabase_WithdrawQueue()
            wcs.Do_deposit()
            # Deal withdraw
            wcs.Assign_Packbox_to_Order()
            wcs.Pick_Pack_Port_Pair()
            # communicate gcodes sender
            gcode_senders_spin_once()
            time.sleep(0.5)

wcs_queue_deposit = multiprocessing.Queue()
# wcs_queue_withdraw = multiprocessing.Queue()
wcs_queue_takeout = multiprocessing.Queue()

def Start_WCS_Process():
    p = multiprocessing.Process(target=WCS_Main, args=(wcs_queue_deposit, ))
    p.start() 
    Logger.Info('WCS is running on new process.....')

    # https://pymotw.com/2/multiprocessing/communication.html#communication-between-processes




