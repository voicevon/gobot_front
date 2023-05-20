from twh_database.db_withdraw_order import DB_WithdrawOrder

from twh_wcs.twhwcs_common.twh_robot_loop_porter import Twh_LoopPorter
from twh_wcs.von.wcs.pick_placer.manual_pick_placer import Manual_PickPlacer
from twh_wcs.von.wcs.order_item import Wcs_OrderItemBase

from von.logger import Logger

class Twh_OrderItem(Wcs_OrderItemBase):

    def __init__(self, db_doc_id:int, porter:Twh_LoopPorter, pick_placer:Manual_PickPlacer) -> None:
        super().__init__(db_doc_id)
        # self.doc_id = db_doc_id
        self.DentalLocation = 'ur1'
        self.row :int
        self.col:int
        self.layer:int
        self.__linked_loop_porter = porter
        self.__linked_pick_placer = pick_placer
        self.__got_start_command = False

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

    def SpinOnce(self):
        Logger.Debug('loop_manual warehouse:: order_item:: SpinOnce()')
        # Logger.Print('state', self._state)
        if self._state == 'idle':
            self._state = 'inside_loop_porter'

        if self._state == 'inside_loop_porter':
            self.__got_start_command = True  # for debug only
            if self.__got_start_command:
                # loop_porter =  self._get_idle_porter()
                # if loop_porter is not None:
                #     loop_porter.MoveToGate()
                if self.__linked_loop_porter.GetState() == 'idle':
                    # P2a: start loop_porter
                    self.__linked_loop_porter.CarryToGate(self.col, self.layer)
                    # P2b: update my state
                    self._state = 'inside_loop_porter_moving'
                    # P2c: update parent state
                    # Q: How to inform parent?
                    # A: Parent will query my state.

        if self._state == 'inside_loop_porter_moving':
            if self.__linked_loop_porter.GetState() == 'ready':
                self._state = 'loop_porter_gate'


        if self._state == 'loop_porter_gate':
            pick_row = 1
            pick_layer = 2
            place_cell = 3
            pick_at = (pick_row, pick_layer)
            self.__linked_pick_placer.Start(pick_at, place_cell)
            self._state = 'loop_porter_gate_picking'

        if self._state == 'loop_porter_gate_picking':
            if self.__linked_loop_porter.GetState()=='eef_done':
                self._state = 'loop_porter_done'

        if self._state == 'loop_porter_done':
            # Q: Need to wait tube conveyer be idle ?
            # A: Don't know now.
            pass

