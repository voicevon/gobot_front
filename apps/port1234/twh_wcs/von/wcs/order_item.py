from abc import ABC, abstractmethod

class Wcs_OrderItemBase(ABC):
    
    def __init__(self, warehouse_id:str, db_doc_id:int) -> None:
        self.doc_id = db_doc_id
        self._state = 'idle'
        self._warehouse_id = warehouse_id

    def GetState(self) ->str:
        return self._state

    def SpinOnce(self):
        self._run_statemachine()
        
    @abstractmethod
    def StartWithdraw(self):
        pass

    
    @abstractmethod
    def _run_statemachine(self):
        pass