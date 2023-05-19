from twh_database.db_withdraw_order import DB_WithdrawOrder

from twh_wcs.von.wcs.order import Wcs_OrderBase, Wcs_OrderItemBase

from von.logger import Logger


class Twh_OrderItem(Wcs_OrderItemBase):

    def __init__(self, db_doc_id:int) -> None:
        super().__init__(db_doc_id)
        # self.doc_id = db_doc_id
        self.DentalLocation = 'ur1'
        self.row :int
        self.col:int
        self.layer:int
        # self.__located = 'porter'

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


class Twh_Order(Wcs_OrderBase):

    def __init__(self, twh_order_id:str) -> None:
        super().__init__(twh_order_id,1234)
        # self._all_order_items = list[Twh_OrderItem]()
        # self._state = 'idle'
        # self.Order_id = order_id
        self.PackerCell_id = -1

    def AddTooth(self, new_tooth:Twh_OrderItem) -> None:
        self._all_order_items.append(new_tooth)
    
    def FindTooth_from_doc_id(self, doc_id:int) -> Twh_OrderItem:
        for t in self._all_order_items:
            if t.doc_id == doc_id:
                return t # type: ignore
        return None # type: ignore

    def FindTooth_is_in_porter(self, porter_id:int) -> Twh_OrderItem:
        '''
        * porter_id is equal to tooth.row.
        * constraint:  tooth must be located in porter
        '''
        # Logger.Debug('WithdrawOrder:: FindTooth_is_in_porter() ')
        for tooth in self._all_order_items:
            # tooth.PrintOut('WithdrawOrder:: FindTooth_is_in_porter(), _all_order_items.this tooth')
            # Logger.Print('located', tooth.GetLocated())
            if tooth.GetLocated() == 'porter':
                if tooth.row == porter_id:
                    return tooth
        return None # type: ignore

    def HasTooth(self, tooth:Twh_OrderItem) -> bool:
        for t in self._all_order_items:
            if tooth == t:
                return True
        return False
    
    def __get_all_teeth_doc_ids(self):
        doc_ids = []
        for tooth in self._all_order_items:
            doc_ids.append(tooth.doc_id)
        return doc_ids
    
    def SetStateTo(self, new_state:str, write_to_db:bool):
        '''
        * 'idle', 
        * 'feeding', 
        * 'fullfilled', 
        * 'wms_shipping'
        * 'wcs_shipping'
        * 'shipped'
        '''
        self._state = new_state
        if write_to_db:
                doc_ids = self.__get_all_teeth_doc_ids()
                DB_WithdrawOrder.update_order_state(new_state, doc_ids)

    # remove this from order. should be a method of order_scheduler.  right?
    def Start_PickingPlacing_a_tooth(self) -> bool:
        if self._state == 'idle':
            # this is the first tooth of the order. 
            idle_packer_cell_id =  twh_packers[0].Find_Idle_packer_cell()
            if idle_packer_cell_id == -1:
                return False
            self.PackerCell_id = idle_packer_cell_id
            twh_packers[0].StartFeeding_LockPackerCell(idle_packer_cell_id)
            self.SetStateTo('feeding', write_to_db=True)
        return True    
            
    # def GetState(self) -> str:
    #     return self._state

    # def IsFullFilled(self) -> bool:
    #     for t in self._all_order_items:
    #         if t.GetLocated() != 'packer':
    #             return False
            
    #     return True

    def SpinOnce(self) -> bool:
        '''
        return:
            * True: I am complete shipped out, and has been deleted from database.
            * False: I am not shipped.
        '''
        if self._state == 'idle':
            return False
        if self._state == 'feeding':
            if self.IsFullFilled():
                doc_ids = self.__get_all_teeth_doc_ids()
                DB_WithdrawOrder.update_order_state('fullfilled', doc_ids)
                return False
        if self._state == 'fullfilled':
            return False
        
        if self._state == 'wms_shipping':
            # if self.__twh_shipper.IsShipping():
            if twh_shippers[0].IsShipping():
                return False
            # self.__twh_packer.StartShipping(self.PackerCell_id)
            twh_packers[0].StartShipping(self.PackerCell_id)
            # self.__twh_shipper.StartShipping() 
            twh_shippers[0].StartShipping() 
            # multiple orders is in the state of 'wms_shipping'
            doc_ids = self.__get_all_teeth_doc_ids()
            DB_WithdrawOrder.update_order_state('wcs_shipping', doc_ids)
            return False

        if self._state == 'wcs_shipping':
            # if self.__twh_shipper.Get_Shipout_button_value()=='ON':
            if twh_shippers[0].Get_Shipout_button_value()=='ON':
                # self.__twh_shipper.EndShipping()
                twh_shippers[0].EndShipping()

                DB_WithdrawOrder.delete_by_order_id(self.order_id)
                twh_packers[0].Release_packer_cell(self.PackerCell_id)
                return True
        return False


