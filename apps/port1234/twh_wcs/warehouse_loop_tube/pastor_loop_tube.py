from twh_wcs.von.wcs.pastor_base import PastorBase
from twh_wcs.twh_robot.twh_loop_porter import Twh_LoopPorter
from twh_wcs.von.wcs.workers.conveyor.simple_tube import SimpleTubeConveyor
from twh_wcs.warehouse_loop_tube.twh_order import  Twh_Order, Twh_OrderItem
from twh_wcs.warehouse_loop_tube.twh_order_manager import Twh_OrderManager


from von.logger import Logger
import multiprocessing


class Pastor_LoopTubeSystem(PastorBase):

    def __init__(self, wcs_instance_id:str, deposit_queue:multiprocessing.Queue) -> None:
        '''
        Current Version hardwares
            Single loop_porter  in the loop_porter_list
            Single tube_conveyor

        '''
        self.__twh_order_manager = Twh_OrderManager(wcs_instance_id)
        super().__init__(wcs_instance_id, deposit_queue, self.__twh_order_manager)

    #//////////////////////////////////////////////////////////////////////////////////////
    #  override methods
    #//////////////////////////////////////////////////////////////////////////////////////
    def _start_deposit(self, new_deposit_request):
        pass
 
    def _end_deposit(self):
        pass

    # def __Do_deposit_begin(self, new_deposit_request):
    #     Logger.Info(twh_factories[self._warehouse_id]['name'] + " -- Twh_WarehouseControlSystem::Do_deposit() ")
    #     Logger.Print("new_deposit_request", new_deposit_request)
    #     # the loop-porter will move to col-position
    #     row_id = new_deposit_request['row']
    #     col_id = new_deposit_request['col']
    #     layer_id = new_deposit_request['layer']
    #     Logger.Print("row_id", row_id)
    #     Logger.Print("porters count", len(self.loop_porters))
    #     porter = self.loop_porters[row_id]
    #     self.__depositing_porter = porter
    #     Logger.Print('layer_id', layer_id)
    #     porter.MoveTo(col_id, layer_id)
    #     porter.show_layer_led()
    #     # Logger.Print("Twh_WarehouseControlSystem::Do_deposit()    point", 99)

    # def Do_deposit_end(self):
    #     self.__depositing_porter.turn_off_leds()
    #     self.__depositing_porter.SetStateTo('idle')
    
    # def __Pair_idle_porter_and_tooth(self) -> tuple[Twh_LoopPorter, Twh_Order, Twh_OrderItem]: 
    #     # Logger.Debug("Twh_WarehouseControlSystem::__Withdraw_Pair_porter_tooth()")
    #     # Logger.Print("porters count", len(self.__porters))
    #     for porter in self.loop_porters:
    #         # Logger.Print("porter state", porter.GetState())
    #         if porter.GetState() == 'idle':
    #             # Logger.Print('__Pair_idle_porter_and_tooth()  Found idle porter, porter_id', porter.id)
    #             tooth, order = self.__twh_orders.FindTooth_is_in_porter_from_all_orders(porter.id)
    #             if tooth is not None:
    #                 Logger.Print('Paired.   found tooth is in the porter, col', tooth.col)
    #                 return porter,order, tooth
    #     return None, None, None # type: ignore

    # def __try_to_withdraw_a_tooth(self):
    #     # setp 1:  Pair idle_porter, picking_tooth
    #     idle_porter, picking_order, picking_tooth = self.__Pair_idle_porter_and_tooth()
    #     if picking_tooth is None:
    #         # Logger.Info("__try_to_withdraw_a_tooth()   picking_tooth is None")
    #         return
        
    #     # step2: whether or not:  the order linked to a packer-cell  
    #     # Logger.Print("__try_to_withdraw_a_tooth()     idle_porter ", idle_porter.id)
    #     is_ok = picking_order.Start_PickingPlacing_a_tooth()
    #     if is_ok:
    #         Logger.Print("__try_to_withdraw_a_tooth()     Start loop_porting ", idle_porter.id)
    #         idle_porter.Start_Porting(picking_tooth, picking_order)  
        
    # def _state_machine_main(self):
    #     if self._wcs_state == 'idle':
    #         if self._deposit_queue.empty():
    #             self._wcs_state = 'withdraw_dispaching'
    #         else:
    #             new_request = self_deposite_queue.get()
    #             if new_request['message_type'] == 'deposit_begin':
    #                 self.__Do_deposit_begin(new_request)
    #             else:
    #                 Logger.Error('state_machine_main(),   wrong deposite request')
    #             self._wcs_state = 'deposit_begin'

    #     if self._wcs_state == 'deposit_begin':
    #             # operator is putting teeth into packer-cell
    #             # when he/she is complete the task, 
    #             # he/she will press a web page button. 
    #             # and queue_web_requst will create a new message
    #             if self._deposit_queue.empty():
    #                 return
    #             new_request = self._deposit_queue.get()   # will be blocked if queue is empty
    #             if new_request['message_type'] == 'deposit_end':
    #                 self.Do_deposit_end()
    #             else:
    #                 Logger.Error('state_machine_main() try to end deposit ,   wrong deposite request')
    #                 Logger.Print('new_request["message_type"]',new_request['message_type'])
    #             self._wcs_state = 'idle'
    
    #     if self._wcs_state == 'withdraw_dispaching':
    #         if self.__twh_orders_scheduler.GetWithdrawOrdersCount() == 0:
    #             self._wcs_state = 'idle'
    #             return
            
    #         # try to find idle_porter matched tooth in order. and pair (porter, tooth)
    #         # Logger.Debug("state_machine_main()  withdraw_dispaching ")
    #         self.__try_to_withdraw_a_tooth()

    #         # try to find ready_porter
    #         # Q: How about idle_loop_porter?  Is an idle_loop_porter same to ready_loop_porter?
    #         # A: 
    #         ready_porter = self.Find_LoopPorter_ready()
    #         if ready_porter is None:
    #             # all porters are busy
    #             # Logger.Info("TwhWcs_Unit::__state_machine_main() on state==withdraw_dispaching,  all porters are busy")
    #             return
    #         ready_porter.show_layer_led()
    #         porting_order, porting_tooth = ready_porter.Get_Porting_Order_and_Item()

    #         self.__picking_ready_porter = ready_porter
    #         self._wcs_state = 'picking_placing'
                    


