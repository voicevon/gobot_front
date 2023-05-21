from abc import ABC, abstractmethod

class Wcs_WorkerBase(ABC):
    
    def __init__(self, warehouse_id:str) -> None:
        self.warehouse_id = warehouse_id

    @abstractmethod
    def SpinOnce(self):
        pass
 