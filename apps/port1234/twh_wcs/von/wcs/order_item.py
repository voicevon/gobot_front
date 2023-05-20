from abc import ABC, abstractmethod

class Wcs_OrderItemBase(ABC):
    
    def __init__(self, db_doc_id:int) -> None:
        self.doc_id = db_doc_id
        # self.__located = 'unknown'
        self._state = 'idle'


    # def GetLocated(self) -> str:
    #     '''
    #     'porter', 'worker_hand','packer_cell'
    #     '''
    #     return self.__located
    
    @abstractmethod
    def SpinOnce(self):
        pass