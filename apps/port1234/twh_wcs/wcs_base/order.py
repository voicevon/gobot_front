from twh_wcs.wcs_base.packer import Wcs_PackerBase
from twh_wcs.wcs_base.shipper import Wcs_ShipperBase

from abc import ABC, abstractmethod



class Wcs_OrderItemBase(ABC):
    
    def __init__(self, db_doc_id:int) -> None:
        self.doc_id = db_doc_id
        self.__located = 'porter'

    def GetLocated(self) -> str:
        '''
        'porter', 'worker_hand','packer_cell'
        '''
        return self.__located
    
    
class Wcs_OrderBase:
    # def __init__(self, order_id:int, packer:Wcs_PackerBase, shipper:Wcs_ShipperBase) -> None:
    def __init__(self, order_id:int) -> None:
        self.order_id = order_id
        self._all_order_items = list[Wcs_OrderItemBase]()
        self.__state = 'idle'


    def GetState(self) -> str:
        return self.__state

    def IsFullFilled(self) -> bool:
        for t in self._all_order_items:
            if t.GetLocated() != 'packer':
                return False
            
        return True