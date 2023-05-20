from abc import ABC, abstractmethod

class Wcs_WorkerBase(ABC):
    
    def __init__(self, owner_id:str) -> None:
        self.Owner_id = owner_id

    @abstractmethod
    def SpinOnce():
        pass
