from twh_database.db_withdraw_order import DB_WithdrawOrder
from twh_wcs.warehouse_loop_manual_pack.twh_order_item import Twh_OrderItem
from twh_wcs.wcs_workers_factory import g_workers
from twh_wcs.wcs_deck_factory import DeckGroupFactory

from twh_wcs.von.wcs.deck.simple_deck import SimpleDeck
from twh_wcs.von.wcs.order import Wcs_OrderBase

from von.logger import Logger


class Twh_Order(Wcs_OrderBase):

    def __init__(self, warehouse_id:str, twh_order_id:int) -> None:
        super().__init__(warehouse_id, twh_order_id)
        self.__linked_output_deck: SimpleDeck
        self.__linked_shipper = g_workers[warehouse_id].shippers[0]
    
    def UpdateStateToDb(self, new_state:str):
            doc_ids = self._get_all_teeth_doc_ids()
            DB_WithdrawOrder.update_order_state(new_state, doc_ids)

    def SystemeRecoverAfterReset(self, new_state:str):
         self._state = new_state
         
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
                new_tooth = Twh_OrderItem(self._warehouse_id, db_tooth.doc_id)
                new_tooth.DentalLocation = db_tooth['location']
                new_tooth.row = db_tooth['row']
                new_tooth.col = db_tooth['col']
                new_tooth.layer = db_tooth['layer']
                new_tooth.LinkPeripheral(loop_porter, picker, self.__linked_output_deck)
                self._all_order_items.append(new_tooth)
                if not has_printed_title:
                    Logger.Info('Twh_Order::_create_order_items()')
                    has_printed_title = True
                Logger.Print('added  new tooth,  DentalLocation', new_tooth.DentalLocation)

    def _run_statemachine(self):
        if self._state == 'idle':
            idle_decks = DeckGroupFactory.FindIdleDecks(self._warehouse_id, 'packer')
            if len(idle_decks) > 0:
                self.__linked_output_deck = idle_decks[0]
                self.__linked_output_deck.SetStateTo('loading')
                # self.__linked_output_deck.TurnOn_BeingAssigned_Led()
                self._state = 'got_deck'

        if self._state == 'got_deck':
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
            self.__linked_output_deck.SetStateTo('loaded')
            # for future version purposed.
            self._state = 'packed'

        if self._state == 'packed':
             if self.__linked_shipper.GetState() == 'shipped':
                  self._state = 'shipped'



