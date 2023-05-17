from twh_wcs.wcs_base.packer import Wcs_PackerBase
from twh_wcs.wcs_base.shipper import Wcs_ShipperBase





class Wcs_OrderItemBase():
    
    def __init__(self, db_doc_id:int) -> None:
        self.doc_id = db_doc_id
        self.__located = 'porter'

    def GetLocated(self) -> str:
        '''
        'porter', 'worker_hand','packer_cell'
        '''
        return self.__located
    
    
class Wcs_OrderBase:
    def __init__(self, order_id:int, packer:Wcs_PackerBase, shipper:Wcs_ShipperBase) -> None:
        self.Order_id = order_id
