import multiprocessing


class WcsUnitBase:

    def __init__(self, wcs_unit_id:str, deposit_queue:multiprocessing.Queue) -> None:
        self._wcs_unit_id = wcs_unit_id
        self._deposit_queue = deposit_queue
        self._wcs_state = 'idle'  
