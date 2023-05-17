from twh_database.db_withdraw_order import DB_WithdrawOrder
from twh_database.bolt_nut import twh_factories

from twh_wcs.twh_robot_packer import TwhRobot_Packer
from twh_wcs.twh_robot_shipper import TwhRobot_Shipper
from twh_wcs.wcs_base.order import Wcs_OrderBase, Wcs_OrderItemBase

from von.logger import Logger




class Twh_OrderItem(Wcs_OrderItemBase):
    def __init__(self, db_doc_id:int) -> None:
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


class Twh_WithdrawOrder(Wcs_OrderBase):
    def __init__(self, order_id:int, packer:TwhRobot_Packer, shipper:TwhRobot_Shipper) -> None:
        self.__all_teeth = []
        # self.Order_id = order_id
        self.PackerCell_id = -1
        self.__state = 'idle'
        self.__packer = packer
        self.__shipper = shipper

    def AddTooth(self, new_tooth:Twh_OrderItem) -> None:
        self.__all_teeth.append(new_tooth)
    
    def FindTooth_from_doc_id(self, doc_id:int) -> Twh_OrderItem:
        for t in self.__all_teeth:
            if t.doc_id == doc_id:
                return t
        return None # type: ignore

    def FindTooth_is_in_porter(self, porter_id:int) -> Twh_OrderItem:
        '''
        * porter_id is equal to tooth.row.
        * constraint:  tooth must be located in porter
        '''
        # Logger.Debug('WithdrawOrder:: FindTooth_is_in_porter() ')
        for tooth in self.__all_teeth:
            # tooth.PrintOut('WithdrawOrder:: FindTooth_is_in_porter(), __all_teeth.this tooth')
            # Logger.Print('located', tooth.GetLocated())
            if tooth.GetLocated() == 'porter':
                if tooth.row == porter_id:
                    return tooth
        return None # type: ignore

    def HasTooth(self, tooth:Twh_OrderItem) -> bool:
        for t in self.__all_teeth:
            if tooth == t:
                return True
        return False
    
    def __get_all_teeth_doc_ids(self):
        doc_ids = []
        for tooth in self.__all_teeth:
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
        self.__state = new_state
        if write_to_db:
                doc_ids = self.__get_all_teeth_doc_ids()
                DB_WithdrawOrder.update_order_state(new_state, doc_ids)

    #TODO: a better name:  Start_PickingPlacing()   StartTransfer()   
    # def Start_PickingPlacing_a_tooth(self, idle_packer_cell_id:int):
    def Start_PickingPlacing_a_tooth(self) -> bool:
        if self.__state == 'idle':
            # this is the first tooth of the order. 
            idle_packer_cell_id =  self.__packer.Find_Idle_packer_cell()
            if idle_packer_cell_id == -1:
                return False
            self.PackerCell_id = idle_packer_cell_id
            self.__packer.StartFeeding_LockPackerCell(idle_packer_cell_id)
            self.SetStateTo('feeding', write_to_db=True)
        return True    
            
    def GetState(self) -> str:
        return self.__state

    def IsFullFilled(self) -> bool:
        for t in self.__all_teeth:
            if t.GetLocated() != 'packer':
                return False
            
        return True

    def SpinOnce(self) -> bool:
        '''
        return:
            * True: I am complete shipped out, and has been deleted from database.
            * False: I am not shipped.
        '''
        if self.__state == 'idle':
            return False
        if self.__state == 'feeding':
            if self.IsFullFilled():
                doc_ids = self.__get_all_teeth_doc_ids()
                DB_WithdrawOrder.update_order_state('fullfilled', doc_ids)
                return False
        if self.__state == 'fullfilled':
            return False
        
        if self.__state == 'wms_shipping':
            if self.__shipper.IsShipping():
                return False
            self.__packer.StartShipping(self.PackerCell_id)
            self.__shipper.StartShipping() 
            # multiple orders is in the state of 'wms_shipping'
            doc_ids = self.__get_all_teeth_doc_ids()
            DB_WithdrawOrder.update_order_state('wcs_shipping', doc_ids)
            return False

        if self.__state == 'wcs_shipping':
            if self.__shipper.Get_Shipout_button_value()=='ON':
                self.__shipper.EndShipping()

                DB_WithdrawOrder.delete_by_order_id(self.Order_id)
                self.__packer.Release_packer_cell(self.PackerCell_id)
                return True
        return False


