

class BinaryOutput_Basic:
    def __init__(self) -> None:
        self._state = 'OFF'
    
    def _set_state(self, is_turn_on:bool):
        if is_turn_on:
            self._state = 'ON'
        else:
            self._state = 'OFF'
    
    def Turn_On(self):
        self._state = 'ON'

    def Turn_Off(self):
        self._state = 'OFF'

    def GetState(self) -> str:
        return self._state
