from von.logger import Logger
from abc import ABC, abstractmethod

class Wcs_WorkerBase(ABC):
    
    def __init__(self, warehouse_id:str) -> None:
        '''
        TODO:  remove warehouse_id
        '''
        self.warehouse_id = warehouse_id

    @abstractmethod
    def SpinOnce(self):
        pass
 
    # @abstractmethod
    def _print_out(self):
        Logger.Print("Warehouse_id", self.warehouse_id)


