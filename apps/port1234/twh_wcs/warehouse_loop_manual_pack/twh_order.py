from twh_database.db_withdraw_order import DB_WithdrawOrder
from twh_wcs.warehouse_loop_manual_pack.twh_order_item import Twh_OrderItem

# from twh_wcs.von.wcs.packer.packer import Wcs_PackerBase
# from twh_wcs.twh_robot.twh_thames_bridge_packer import Twh_ThamesBridge_Packer
from twh_wcs.von.wcs.deck.simple_deck import SimpleDeck
from twh_wcs.von.wcs.order import Wcs_OrderBase
from twh_wcs.wcs_workers_factory import g_workers, WorkersFactory
from twh_wcs.wcs_deck_factory import DeckGroupFactory

from von.logger import Logger


class Twh_Order(Wcs_OrderBase):
    def __init__(self, warehouse_id:str, twh_order_id:int) -> None:
        super().__init__(warehouse_id, twh_order_id)
        self.__linked_packer: SimpleDeck
        self.__linked_shipper = g_workers[warehouse_id].shippers[0]
    
    # def SetStateTo(self, new_state:str, write_to_db:bool):
        # self._state = new_state
        # if write_to_db:
        #         doc_ids = self._get_all_teeth_doc_ids()
        #         DB_WithdrawOrder.update_order_state(new_state, doc_ids)

    # def Get_linked_packer(self) -> SimplePacker:
    #     return self.__linked_packer
    
    def UpdateStateToDb(self, new_state:str):
            doc_ids = self._get_all_teeth_doc_ids()
            DB_WithdrawOrder.update_order_state(new_state, doc_ids)

    def SystemeRecoverAfterReset(self, new_state:str):
         self._state = new_state
         
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


    def _create_order_items(self):
        '''
        turorial note: https://tinydb.readthedocs.io/en/latest/usage.html
        The TinyDB query cache doesn't check if the underlying storage that the database uses has been modified by an external process. 
        In this case the query cache may return outdated results. 
        To clear the cache and read data from the storage again you can use db.clear_cache().
        '''
        # Logger.Debug('loop-manual warehouse:: Twh_OrderManager:: renew_order_state_from_database()')
        DB_WithdrawOrder.table_withdraw_order.clear_cache()
        db_order_teeth =  DB_WithdrawOrder.table_withdraw_order.all()
        has_printed_title = False
        for db_tooth in db_order_teeth:
            picker = g_workers[self._warehouse_id].pick_placers[0]
            if db_tooth['order_id'] == self._order_id:
                loop_porter = g_workers[self._warehouse_id].loop_porters[db_tooth['row']]
                new_tooth = Twh_OrderItem(self._warehouse_id, db_tooth.doc_id, loop_porter, picker, self.__linked_packer)
                new_tooth.DentalLocation = db_tooth['location']
                new_tooth.row = db_tooth['row']
                new_tooth.col = db_tooth['col']
                new_tooth.layer = db_tooth['layer']
                self._all_order_items.append(new_tooth)
                if not has_printed_title:
                    Logger.Info('Twh_Order::_create_order_items()')
                    has_printed_title = True
                Logger.Print('added  new tooth,  DentalLocation', new_tooth.DentalLocation)

    def _run_statemachine(self):
        if self._state == 'idle':
            # idle_packers = WorkersFactory.FindIdlePackers(self._warehouse_id, 'packer')
            idle_packers = DeckGroupFactory.FindIdleDecks(self._warehouse_id, 'packer')
            if len(idle_packers) > 0:
                self.__linked_packer = idle_packers[0]
                self.__linked_packer.SetStateTo('feeding')
                self._state = 'got_packer_cell'

        if self._state == 'got_packer_cell':
                self._create_order_items()
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


