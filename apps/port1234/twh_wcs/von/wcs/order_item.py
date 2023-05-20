from abc import ABC, abstractmethod

class Wcs_OrderItemBase(ABC):
    
    def __init__(self, db_doc_id:int) -> None:
        self.doc_id = db_doc_id
        # self.__located = 'unknown'
        self._state = 'idle'

    def GetState(self) ->str:
        return self._state

    @abstractmethod
    def StartWithdraw(self):
        pass

    @abstractmethod
    def SpinOnce(self):
        pass