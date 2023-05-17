from twh_business_logical.withdraw_order import  WithdrawOrdersManager, WithdrawOrder, OrderTooth
from wcs_robots.twh_robot_packer import TwhRobot_Packer
from wcs_robots.twh_robot_shipper import TwhRobot_Shipper

from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
import multiprocessing
from abc import abstractmethod
from von.mqtt.mqtt_agent import g_mqtt


class WcsUnitBase:

    def __init__(self, wcs_unit_id:str, deposit_queue:multiprocessing.Queue) -> None:
        self._wcs_unit_id = wcs_unit_id
        self._deposit_queue = deposit_queue
        self._wcs_state = 'idle'  
        self.__porters = []
        # __button_pick is a green button sit on packer.
        self.__button_pick = RemoteVar_mqtt('twh/' + wcs_unit_id + '/packer/button/pick','idle')
        # __button_pack is a blue button sit on packer.
        self.__button_shipped = RemoteVar_mqtt('twh/' + wcs_unit_id + '/packer/button/pack','idle')
        self.__packer = TwhRobot_Packer()
        self.__shipper = TwhRobot_Shipper(button_shipped=self.__button_shipped)
        self.__withdraw_orders_manager = WithdrawOrdersManager(wcs_unit_id, self.__packer, self.__shipper)
        self.__showing_wcs_state = ''

    @abstractmethod
    def __state_machine_main():
        ...

    def SpinOnce(self) ->str:
        '''
        return:  _wcs_state
        '''
        # Logger.Debug("TwhWcs_Unit::SpinOnce()")
        # Logger.Print("my twh_id", self._wcs_unit_id)
        self.__state_machine_main() 
        self.__withdraw_orders_manager.SpinOnce()
        if self.__showing_wcs_state != self._wcs_state:
            showing_wcs_state = self._wcs_state
            g_mqtt.publish('twh/' + self._wcs_unit_id + '/wcs_state',showing_wcs_state)
        return self._wcs_state

    def all_loop_porter_are_idle(self) -> bool:
        for porter in self.__porters:
            if porter.GetState() != 'idle':
                return False
        return True


