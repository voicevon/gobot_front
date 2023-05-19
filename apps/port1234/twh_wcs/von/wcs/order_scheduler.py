# from twh_wcs.wcs_base.packer import Wcs_PackerBase
# from twh_wcs.wcs_base.shipper import Wcs_ShipperBase

# from abc import ABC, abstractmethod

# class Wcs_OrderSchedulerBase(ABC):

    # # def __init__(self, wcs_unit_id:str, packer:Wcs_PackerBase, shipper:Wcs_ShipperBase) -> None:
    # def __init__(self, wcs_unit_id:str, order_id:int) -> None:
    #     self.wcs_unit_id = wcs_unit_id
    #     self.order_id = order_id
    #     self._all_order_items = list[Wcs_OrderItemBase]()
    #     self.__state = 'idle'


    # def GetState(self) -> str:
    #     return self.__state

    # def IsFullFilled(self) -> bool:
    #     for t in self._all_order_items:
    #         if t.GetLocated() != 'packer':
    #             return False
            
    #     return True
    # @abstractmethod
    # def SpinOnce(self):
    #     pass

    # @abstractmethod
    # def GetWithdrawOrdersCount(self) -> int:
    #     pass


        