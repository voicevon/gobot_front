from twh_wcs.von.wcs.order_item import Wcs_OrderItemBase

from von.logger import Logger
from abc import ABC, abstractmethod

    
class Wcs_OrderBase(ABC):

    def __init__(self, warehouse_id:str, order_id:int) -> None:
        self.warehouse_id = warehouse_id
        self.order_id = order_id
        self._all_order_items = list[Wcs_OrderItemBase]()
        self._state = 'idle'
    def GetState(self) -> str:
        return self._state
    
    def AddItem(self, new_item:Wcs_OrderItemBase) -> None:
        self._all_order_items.append(new_item)

    def FindItem_from_doc_id(self, doc_id:int) -> Wcs_OrderItemBase:
        for item in self._all_order_items:
            if item.doc_id == doc_id:
                return item 
        return None # type: ignore
    
    def _get_all_teeth_doc_ids(self):
        doc_ids = []
        for tooth in self._all_order_items:
            doc_ids.append(tooth.doc_id)
        return doc_ids
    
    def _all_items_is_in_state(self, the_state:str) -> bool:
        for i in self._all_order_items:
            if i.GetState() != the_state:
                return False
        return True

    def SpinOnce(self):
        for item in self._all_order_items:
            # Logger.Print("item---id", item.doc_id)
            item.SpinOnce()
        self._run_statemachine()

    @abstractmethod
    def _run_statemachine(self) -> int:
        pass


      