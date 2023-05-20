from twh_database.db_withdraw_order import DB_WithdrawOrder

from twh_wcs.twh_robot.twh_loop_porter import Twh_LoopPorter
from twh_wcs.von.wcs.pick_placer.manual_pick_placer import Manual_PickPlacer
from twh_wcs.von.wcs.order_item import Wcs_OrderItemBase

from twh_wcs.wcs_workers_factory import g_workers

from von.logger import Logger

class Twh_OrderItem(Wcs_OrderItemBase):

    def __init__(self, warehouse_id:str, db_doc_id:int, porter:Twh_LoopPorter, pick_placer:Manual_PickPlacer) -> None:
        super().__init__(warehouse_id, db_doc_id)
        # self.doc_id = db_doc_id
        self.DentalLocation = 'ur1'
        self.row :int
        self.col:int
        self.layer:int
        self.__linked_loop_porter = porter
        self.__linked_pick_placer = pick_placer
        # self.__got_start_command = False


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
        else:
            Logger.Error("OrderItem::StartWithdraw()    I am not on idle, can not start")

    def _run_statemachine(self):
        Logger.Debug('loop_manual warehouse:: order_item:: SpinOnce()')
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
                # turn on led on looper gate
                self.__linked_loop_porter._show_layer_led()
                # turn on led on packer_cell
                self.l
                place_cell = 3
                pick_at = (self.row, self.layer)
                self.__linked_pick_placer.Start(pick_at, place_cell)
                self._state = 'picking_placing'

        if self._state == 'picking_placing':
            if self.__linked_loop_porter.GetState()=='picked_placed':
                self.__linked_loop_porter.ResetStatemachine()
                self._state = 'ported'

        if self._state == 'ported':
            # Q: Need to wait tube conveyer be idle ?
            # A: Don't know now.
            pass

