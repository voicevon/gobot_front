from von.logger import Logger

class Wcs_ShipperBase:

    def __init__(self) -> None:
        self._state = 'idle'

    def GetState(self) -> str:
        return self._state
    
    def StartShipping(self):
        if self._state == 'idle':
            self._state = 'shipping'
        else:
            Logger.Error('Wcs_ShipperBase::StartShipping()   I am not on idle')
            # Logger.Halt()
    
    def SpinOnce(self):
        blue_button_is_pressed = True
        if self._state == 'shipping':
            if blue_button_is_pressed:
                self._state = 'shipped'