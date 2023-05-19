from twh_wcs.von.wcs.order_item import Wcs_OrderItemBase

from abc import ABC, abstractmethod


    
    
class Wcs_OrderBase:
    def __init__(self, wcs_unit_id:str, order_id:int) -> None:
        self.wcs_unit_id = wcs_unit_id
        self.order_id = order_id
        self._all_order_items = list[Wcs_OrderItemBase]()
        self._state = 'idle'
        '''
        * 'idle', 
        * 'feeding', 
        * 'fullfilled', 
        * 'wms_shipping' ??
        * 'wcs_shipping' ??
        * 'shipped'
        '''
    def GetState(self) -> str:
        return self._state
    
    def AddItem(self, new_item:Wcs_OrderItemBase) -> None:
        self._all_order_items.append(new_item)

    def FindItem_from_doc_id(self, doc_id:int) -> Wcs_OrderItemBase:
        for item in self._all_order_items:
            if item.doc_id == doc_id:
                return item 
        return None # type: ignore
    
    def HasItem(self, item:Wcs_OrderItemBase) -> bool:
        for i in self._all_order_items:
            if item == i:
                return True
        return False
    
    def IsFullFilled(self) -> bool:
        return False
        # for t in self._all_order_items:
        #     if t.GetLocated() != 'packer':
        #         return False
        # return True
    
    
    @abstractmethod
    def SpinOnce(self):
        pass

    # @abstractmethod
    # def GetWithdrawOrdersCount(self) -> int:
    #     pass


      