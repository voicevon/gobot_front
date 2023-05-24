from twh_database.db_withdraw_order import DB_WithdrawOrder

from twh_wcs.twh_robot.twh_loop_porter import Twh_LoopPorter
from twh_wcs.wcs_warehouse_factory import g_warehouses

from twh_wcs.von.wcs.deck.simple_deck import SimpleDeck
from twh_wcs.von.wcs.workers.pick_placer.manual_pick_placer import Manual_PickPlacer
from twh_wcs.von.wcs.order_item import Wcs_OrderItemBase


from von.logger import Logger

class Twh_OrderItem(Wcs_OrderItemBase):

    def __init__(self, warehouse_id:str, db_doc_id:int) -> None:
        super().__init__(warehouse_id, db_doc_id)
        # self.doc_id = db_doc_id
        self.DentalLocation = 'ur1'
        self.row :int
        self.col:int
        self.layer:int

    def LinkPeripheral(self,porter:Twh_LoopPorter, pick_placer:Manual_PickPlacer, deck:SimpleDeck):
        self.__linked_loop_porter = porter
        self.__linked_pick_placer = pick_placer
        self.__linked_deck = deck
        leds_key = 'porter' + str(self.row) + "_leds"
        self.__linked_picking_led = g_warehouses[self._warehouse_id].components_take.binary_outputs[leds_key].Gates[self.layer]
        self.__linked_placing_led = g_warehouses[self._warehouse_id].components_take.binary_outputs['placing_leds'].Gates[deck._index]
 

    # TODO:  remove this
    def TransferToLocated(self, new_located:str, write_to_db:bool) -> None:
        self.__located = new_located
        if write_to_db:
            DB_WithdrawOrder.update_tooth_located(self.doc_id, new_located)
    
    def PrintOut(self, title:str):
        Logger.Info(title)
        Logger.Print('doc_id', self.doc_id)
        Logger.Print('DentalLocation', self.DentalLocation)
        Logger.Print('__located', self.__located)
        Logger.Print('row', self.row)
        Logger.Print('col', self.col)
        Logger.Print('layer', self.layer)

    def StartWithdraw(self):
        if self._state == 'idle':
            self._state = 'started'

            # Logger.Debug('Twh_OrderItem::StartWithdraw()' + str(self.doc_id))
        else:
            Logger.Error("OrderItem::StartWithdraw()    I am not on idle, can not start")

    def _run_statemachine(self):
        # Logger.Debug('loop_manual warehouse:: order_item:: SpinOnce()')
        # Logger.Print('state', self._state)
        if self._state == 'idle':
            # deal at StartWithdraw()
            return
        if self._state == 'started':
            if self.__linked_loop_porter.GetState() == 'idle':
                # P2a: start loop_porter
                self.__linked_loop_porter.Start_CarryToGate(self.col, self.layer)
                # P2b: update my state
                self._state = 'moving'
                # P2c: update parent state
                # Q: How to inform parent?
                # A: Parent will query my state.

        if self._state == 'moving':
            if self.__linked_loop_porter.GetState() == 'ready':
                self._state = 'on_gate'

        if self._state == 'on_gate':
            if self.__linked_pick_placer.GetState() == 'idle':
                # turn on led-pair:  item led on looper gate,  order led on deck ? 
                self.__linked_picking_led._set_state(is_turn_on=True)
                self.__linked_placing_led._set_state(is_turn_on=True)
                # pick_at = (self.row, self.layer)
                self.__linked_pick_placer.Start()
                self._state = 'picking_placing'

        if self._state == 'picking_placing':
            if self.__linked_loop_porter.GetState()=='picked_placed':
                self.__linked_loop_porter.ResetStatemachine()
                self._state = 'ported'

        if self._state == 'ported':
            # Q: Need to wait tube conveyer be idle ?
            # A: Don't know now.
            pass

