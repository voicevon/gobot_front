from twh_wcs.wcs_base.packer import Wcs_PackerBase
from twh_wcs.wcs_base.shipper import Wcs_ShipperBase

from abc import ABC, abstractmethod

class Wcs_OrderSchedulerBase(ABC):

    # def __init__(self, wcs_unit_id:str, packer:Wcs_PackerBase, shipper:Wcs_ShipperBase) -> None:
    def __init__(self, wcs_unit_id:str) -> None:
        self.wcs_unit_id = wcs_unit_id


    @abstractmethod
    def SpinOnce(self):
        pass

    @abstractmethod
    def GetWithdrawOrdersCount(self) -> int:
        pass


        