from twh_database.db_withdraw_order import DB_WithdrawOrder

from twh_wcs.von.wcs.packer.packer import Wcs_PackerBase
from twh_wcs.von.wcs.order import Wcs_OrderBase
from twh_wcs.wcs_workers_factory import g_workers, WorkerFactory

from von.logger import Logger


class Twh_Order(Wcs_OrderBase):
    def __init__(self, warehouse_id:str, twh_order_id:int) -> None:
        super().__init__(warehouse_id, twh_order_id)
        self.__linked_packer: Wcs_PackerBase
        self.__linked_shipper = g_workers[warehouse_id].shippers[0]
    
    def SetStateTo(self, new_state:str, write_to_db:bool):
        '''
        Q: What is the purpose?

        '''
        Logger.Error('Almost an error, At least a warning.     SetStateTo()')
        self._state = new_state
        if write_to_db:
                doc_ids = self._get_all_teeth_doc_ids()
                DB_WithdrawOrder.update_order_state(new_state, doc_ids)

    # remove this from order. should be a method of order_scheduler.  right?
    # def Start_PickingPlacing_a_tooth(self) -> bool:
    #     if self.__state == 'idle':
    #         # this is the first tooth of the order. 
    #         idle_packer_cell_id =  twh_packers[0].Find_Idle_packer_cell()
    #         if idle_packer_cell_id == -1:
    #             return False
    #         self.PackerCell_id = idle_packer_cell_id
    #         twh_packers[0].StartFeeding_LockPackerCell(idle_packer_cell_id)
    #         self.SetStateTo('feeding', write_to_db=True)
    #     return True    
            
    def _run_statemachine(self):
        if self._state == 'idle':
            idle_packers = WorkerFactory.FindIdlePackers(self.warehouse_id)
            if len(idle_packers) > 0:
                self.__linked_packer = idle_packers[0]
                self.__linked_packer.SetStateTo('feeding')
                self._state = 'got_packer_cell'

        if self._state == 'got_packer_cell':
                for item in self._all_order_items:
                    item.StartWithdraw()
                self._state = 'feeding'

        if self._state == 'feeding':
            if self._all_items_is_in_state('ported'):
                doc_ids = self._get_all_teeth_doc_ids()
                DB_WithdrawOrder.update_order_state('fullfilled', doc_ids)
                self._state = 'fullfilled'

        if self._state == 'fullfilled':
            # for future version purposed.
            self._state = 'packed'

        if self._state == 'packed':
             if self.__linked_shipper.GetState() == 'shipped':
                  self._state = 'shipped'
        # if self._state == 'wms_shipping':
        #     # if self.__twh_shipper.IsShipping():
        #     if twh_shippers[0].IsShipping():
        #         return False
        #     # self.__twh_packer.StartShipping(self.PackerCell_id)
        #     twh_packers[0].StartShipping(self.PackerCell_id)
        #     # self.__twh_shipper.StartShipping() 
        #     twh_shippers[0].StartShipping() 
        #     # multiple orders is in the state of 'wms_shipping'
        #     doc_ids = self.__get_all_teeth_doc_ids()
        #     DB_WithdrawOrder.update_order_state('wcs_shipping', doc_ids)
        #     return False

        # if self._state == 'wcs_shipping':
        #     # if self.__twh_shipper.Get_Shipout_button_value()=='ON':
        #     if twh_shippers[0].Get_Shipout_button_value()=='ON':
        #         # self.__twh_shipper.EndShipping()
        #         twh_shippers[0].EndShipping()

        #         DB_WithdrawOrder.delete_by_order_id(self.order_id)
        #         twh_packers[0].Release_packer_cell(self.PackerCell_id)
        #         return True
        # return False


