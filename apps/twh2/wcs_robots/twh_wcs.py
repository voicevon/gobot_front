from wcs_robots.twh_robot_loop_porter import TwhRobot_LoopPorter
from wcs_robots.twh_robot_packer import TwhRobot_Packer
from business_logical.withdraw_order import  WithdrawOrderManager, WithdrawOrder, OrderTooth

import multiprocessing
from von.remote_var_mqtt import RemoteVar_mqtt
from von.mqtt_agent import g_mqtt,g_mqtt_broker_config
from wcs_robots.gcode_sender import gcode_senders_spin_once
import time
from logger import Logger


class Twh_WarehouseControlSystem():

    def __init__(self, queue_deposit:multiprocessing.Queue) -> None:
        self.__porters = [TwhRobot_LoopPorter('221109', 0)]
        for i in range(3):
            new_porter = TwhRobot_LoopPorter('221109', i+1)
            self.__porters.append(new_porter)

        # __button_pick is a green button sit on packer.
        self.__button_pick = RemoteVar_mqtt('twh/221109/packer/button/pick','idle')
        # __button_pack is a blue button sit on packer.
        self.__button_pack = RemoteVar_mqtt('twh/221109/packer/button/pack','idle')
        self.__packer = TwhRobot_Packer(self.__button_pack)
        self.__wcs_state = 'idle'
        self.__order_task_manager = WithdrawOrderManager(self.__packer)
 
    def Find_LoopPorter_ready(self) -> TwhRobot_LoopPorter:
        for porter in self.__porters:
            if porter.GetState() == 'ready':
                return porter
        return None # type: ignore

    def all_loop_porter_are_idle(self) -> bool:
        for porter in self.__porters:
            if porter.GetState() != 'idle':
                return False
        return True

    def Do_deposit_begin(self, new_deposit_request):
        Logger.Info("Twh_WarehouseControlSystem::Do_deposit()")
        Logger.Print("new_deposit_request", new_deposit_request)
        # the loop-porter will move to col-position
        row_id = new_deposit_request['row']
        col_id = new_deposit_request['col']
        layer_id = new_deposit_request['layer']
        Logger.Print("row_id", row_id)
        Logger.Print("porters count", len(self.__porters))
        porter = self.__porters[row_id]
        self.__depositing_porter = porter
        Logger.Print('layer_id', layer_id)
        porter.MoveTo(col_id, layer_id)
        porter.show_layer_led()
        Logger.Print("Twh_WarehouseControlSystem::Do_deposit()    point", 99)

    def Do_deposit_end(self):
        self.__depositing_porter.turn_off_leds()
        self.__depositing_porter.SetStateTo('idle')
    
    def __Pair_idle_porter_and_tooth(self) -> tuple[TwhRobot_LoopPorter, WithdrawOrder, OrderTooth]: 
        # Logger.Debug("Twh_WarehouseControlSystem::__Withdraw_Pair_porter_tooth()")
        for porter in self.__porters:
            if porter.GetState() == 'idle':
                # Logger.Print('found idle porter, porter_id',porter.id)
                tooth, order = self.__order_task_manager.FindTooth_is_in_porter_from_all_order(porter.id)
                if tooth is not None:
                    # Logger.Print('Paired.   found tooth is in the porter, col', tooth.col)
                    return porter,order, tooth
        return None, None, None # type: ignore

    def link_order_tooth_porter_packer(self):
        # setp 1:  Pair idle_porter, picking_tooth
        idle_porter, picking_order, picking_tooth = self.__Pair_idle_porter_and_tooth()
        if picking_tooth is None:
            return
        
        # step2: whether or not:  the order linked to a packer-cell  
        is_ok = picking_order.Start_PickingPlacing_a_tooth()
        if is_ok:
            idle_porter.Start_Porting(picking_tooth, picking_order)  
        

    def state_machine_main(self, queue_web_request:multiprocessing.Queue):
        if self.__wcs_state == 'idle':
            if queue_web_request.empty():
                self.__wcs_state = 'withdraw_dispaching'
            else:
                new_request = queue_web_request.get()
                if new_request['message_type'] == 'deposit_begin':
                    self.Do_deposit_begin(new_request)
                else:
                    Logger.Error('state_machine_main(),   wrong deposite request')
                self.__wcs_state = 'deposit_begin'
        
        if self.__wcs_state == 'deposit_begin':
                # operator is putting teeth into packer-cell
                # when he/she is complete the task, 
                # he/she will press a web page button. 
                # and queue_web_requst will create a new message
                if queue_web_request.empty():
                    return
                new_request = queue_web_request.get()   # will be blocked if queue is empty
                if new_request['message_type'] == 'deposit_end':
                    self.Do_deposit_end()
                else:
                    Logger.Error('state_machine_main() try to end deposit ,   wrong deposite request')
                    Logger.Print('new_request["message_type"]',new_request['message_type'])
                self.__wcs_state = 'idle'
    
        if self.__wcs_state == 'withdraw_dispaching':
            if self.__order_task_manager.GetTasksCount() == 0:
                self.__wcs_state = 'idle'
                return
            
            # try to find idle_porter matched tooth in order. and pair (porter, tooth)
            # Logger.Debug("state_machine_main()  withdraw_dispaching ")
            self.link_order_tooth_porter_packer()

            # try to find ready_porter
            ready_porter = self.Find_LoopPorter_ready()
            if ready_porter is None:
                # all porters are busy
                return
            ready_porter.show_layer_led()
            porting_tooth, porting_order = ready_porter.GetPortingTooth()
            Logger.Print("Ready porter.portingorder.PackerCell_Id", porting_order.PackerCell_id)
            self.__packer.turn_on_packer_cell_led_green(porting_order.PackerCell_id)  

            self.__picking_ready_porter = ready_porter
            self.__wcs_state = 'picking_placing'
                    
        if self.__wcs_state == 'picking_placing':
            if self.__button_pick.get() == 'ON':
                porting_tooth, porting_order = self.__picking_ready_porter.GetPortingTooth()
                porting_tooth.TransferToLocated('packer', write_to_db=True)

                self.__picking_ready_porter.turn_off_leds()
                self.__picking_ready_porter.SetStateTo('idle')
                self.__packer.turn_off_all_packer_cells_led_green()

                self.__wcs_state = 'withdraw_dispaching'

    def SpinOnce(self, deposit_queue:multiprocessing.Queue) ->str:
        '''
        return:  __wcs_state
        '''
        self.state_machine_main(deposit_queue)
        self.__order_task_manager.SpinOnce()

        return self.__wcs_state


def WCS_Main(deposit_queue:multiprocessing.Queue):
        g_mqtt_broker_config.client_id = '20221222'
        g_mqtt.connect_to_broker(g_mqtt_broker_config,wait_connected=True)                # DebugMode, must be turn off.  
        wcs = Twh_WarehouseControlSystem(deposit_queue)
        showing_wcs_state = ''

        while True:
            wcs_state =  wcs.SpinOnce(deposit_queue)
            gcode_senders_spin_once()
            if showing_wcs_state != wcs_state:
                showing_wcs_state = wcs_state
                g_mqtt.publish('twh/221109/wcs_state',showing_wcs_state)
            time.sleep(0.5)


# from wms to wcs
wcs_deposit_queue = multiprocessing.Queue()         

def Start_WCS_Process():
    # p = multiprocessing.Process(target=WCS_Main, args=(wcs_deposit_queue, set_packer_cell_state_queue, packer_cells_state))
    p = multiprocessing.Process(target=WCS_Main, args=(wcs_deposit_queue,))
    p.start() 
    Logger.Info('WCS is running on new process.....')

    # https://pymotw.com/2/multiprocessing/communication.html#communication-between-processes




