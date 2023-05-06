
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt

class TwhRobot_Shipper():
    def __init__(self, button_shipped:RemoteVar_mqtt) -> None:
        self.__button_shipped = button_shipped
        self.__is_shipping = False

    def Get_Shipout_button_value(self):
        return self.__button_shipped.get()
    
    def IsShipping(self) -> bool:
        return self.__is_shipping

    def StartShipping(self):
        self.__is_shipping = True

    def EndShipping(self):
        self.__button_shipped.set('idle')
        self.__is_shipping = False
