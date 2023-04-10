from wcs_robots.twh_robot_loop_porter import TwhRobot_LoopPorter
from wcs_robots.twh_robot_packer import TwhRobot_Packer
from wcs_robots.twh_robot_shipper import TwhRobot_Shipper
from wcs_robots.gcode_sender import gcode_senders_spin_once
from business_logical.withdraw_order import  WithdrawOrderManager, WithdrawOrder, OrderTooth
from business_logical.bolt_nut import twh_factory

import multiprocessing
from von.remote_var_mqtt import RemoteVar_mqtt
from von.mqtt_agent import g_mqtt,g_mqtt_broker_config
import time
from logger import Logger


class TwhWcs_Unit():

    def __init__(self, twh_id:str, deposit_queue:multiprocessing.Queue) -> None:
        self.__porters = [TwhRobot_LoopPorter(twh_id, 0)]
        for i in range(3):
            new_porter = TwhRobot_LoopPorter(twh_id, i+1)
            self.__porters.append(new_porter)

        # __button_pick is a green button sit on packer.
        self.__button_pick = RemoteVar_mqtt('twh/' + twh_id + '/packer/button/pick','idle')
        # __button_pack is a blue button sit on packer.
        self.__button_shipped = RemoteVar_mqtt('twh/' + twh_id + '/packer/button/pack','idle')
        self.__packer = TwhRobot_Packer()
        self.__shipper = TwhRobot_Shipper(button_shipped=self.__button_shipped)
        self.__wcs_state = 'idle'
        self.__order_task_manager = WithdrawOrderManager(twh_id, self.__packer, self.__shipper)
        self.__deposite_queue = deposit_queue
        self.__showing_wcs_state = ''
        self.__twh_id = twh_id
 
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

    def __Do_deposit_begin(self, new_deposit_request):
        Logger.Info(twh_factory[self.__twh_id] + " -- Twh_WarehouseControlSystem::Do_deposit() ")
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
        # Logger.Print("Twh_WarehouseControlSystem::Do_deposit()    point", 99)

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

    def try_to_withdraw_a_tooth(self):
        # setp 1:  Pair idle_porter, picking_tooth
        idle_porter, picking_order, picking_tooth = self.__Pair_idle_porter_and_tooth()
        if picking_tooth is None:
            return
        
        # step2: whether or not:  the order linked to a packer-cell  
        is_ok = picking_order.Start_PickingPlacing_a_tooth()
        if is_ok:
            idle_porter.Start_Porting(picking_tooth, picking_order)  
        
    def __state_machine_main(self):
        if self.__wcs_state == 'idle':
            if self.__deposite_queue.empty():
                self.__wcs_state = 'withdraw_dispaching'
            else:
                new_request = self.__deposite_queue.get()
                if new_request['message_type'] == 'deposit_begin':
                    self.__Do_deposit_begin(new_request)
                else:
                    Logger.Error('state_machine_main(),   wrong deposite request')
                self.__wcs_state = 'deposit_begin'

        if self.__wcs_state == 'deposit_begin':
                # operator is putting teeth into packer-cell
                # when he/she is complete the task, 
                # he/she will press a web page button. 
                # and queue_web_requst will create a new message
                if self.__deposite_queue.empty():
                    return
                new_request = self.__deposite_queue.get()   # will be blocked if queue is empty
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
            self.try_to_withdraw_a_tooth()

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
                self.__button_pick.set('idle')
                porting_tooth, porting_order = self.__picking_ready_porter.GetPortingTooth()
                porting_tooth.TransferToLocated('packer', write_to_db=True)

                self.__picking_ready_porter.turn_off_leds()
                self.__picking_ready_porter.SetStateTo('idle')
                self.__packer.turn_off_all_packer_cells_led_green()

                self.__wcs_state = 'withdraw_dispaching'

    def SpinOnce(self) ->str:
        '''
        return:  __wcs_state
        '''
        self.__state_machine_main() 
        self.__order_task_manager.SpinOnce()
        if self.__showing_wcs_state != self.__wcs_state:
            showing_wcs_state = self.__wcs_state
            g_mqtt.publish('twh/' + self.__twh_id + '/wcs_state',showing_wcs_state)
        return self.__wcs_state


def WCS_Main(deposit_queue:multiprocessing.Queue):
        g_mqtt_broker_config.client_id = '20230355'
        g_mqtt.connect_to_broker(g_mqtt_broker_config)                # DebugMode, must be turn off.  

        all_wcs_units = []
        for twh_id in list(twh_factory.keys()):
            wcs_unit = TwhWcs_Unit(twh_id, deposit_queue)
            all_wcs_units.append(wcs_unit)

        while True:
            for wcs_unit in all_wcs_units:
                wcs_unit.SpinOnce()
            gcode_senders_spin_once()
            time.sleep(0.5)


# from wms to wcs
wcs_deposit_queue = multiprocessing.Queue()         
wcs_is_started = False
def Start_WCS_Process():
    global wcs_is_started
    if wcs_is_started:
        return
    # p = multiprocessing.Process(target=WCS_Main, args=(wcs_deposit_queue, set_packer_cell_state_queue, packer_cells_state))
    wcs_is_started = True
    p = multiprocessing.Process(target=WCS_Main, args=(wcs_deposit_queue,))
    p.start() 
    Logger.Info('WCS is running on new process.....')


    # https://pymotw.com/2/multiprocessing/communication.html#communication-between-processes




