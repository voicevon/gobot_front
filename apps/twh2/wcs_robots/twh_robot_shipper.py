
from von.remote_var_mqtt import RemoteVar_mqtt

class TwhRobot_Shipper():
    def __init__(self, button_shipped:RemoteVar_mqtt) -> None:
        self.__button_shipped = button_shipped

    def Get_Shipout_button_value(self):
        return self.__button_shipped.get()
    
    def Reset_Shipout_button(self):
        self.__button_shipped.set('idle')
