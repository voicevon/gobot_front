from twh_wcs.von.wcs.packer.packer import Wcs_PackerBase
from twh_wcs.von.wcs.shipper.shipper import Wcs_ShipperBase

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
    # def __init__(self, wcs_unit_id:str, packer:Wcs_PackerBase, shipper:Wcs_ShipperBase) -> None:
    def __init__(self, wcs_unit_id:str, order_id:int) -> None:
        self.wcs_unit_id = wcs_unit_id
        self.order_id = order_id
        self._all_order_items = list[Wcs_OrderItemBase]()
        self._state = 'idle'


    def GetState(self) -> str:
        return self._state

    def IsFullFilled(self) -> bool:
        for t in self._all_order_items:
            if t.GetLocated() != 'packer':
                return False
            
        return True
    @abstractmethod
    def SpinOnce(self):
        pass

    @abstractmethod
    def GetWithdrawOrdersCount(self) -> int:
        pass


      