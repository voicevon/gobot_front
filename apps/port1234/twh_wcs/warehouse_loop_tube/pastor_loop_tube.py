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


