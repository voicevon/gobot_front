from twh_wcs.warehouse_loop_manual_pack.twh_order_manager import Twh_OrderManager

from twh_database.bolt_nut import twh_factories
from twh_wcs.von.wcs.workers.shipper.manual_shipper import Manual_Shipper
from twh_wcs.von.wcs.warehouse_base import WarehouseBase
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt

from twh_wcs.wcs_workers_factory import g_workers
from von.logger import Logger
import multiprocessing


class Warehosue_LoopManualPacker(WarehouseBase):

    def __init__(self, wcs_instance_id:str, deposit_queue:multiprocessing.Queue) -> None:
        self.__button_pick = RemoteVar_mqtt('twh/' + wcs_instance_id + '/packer/button/pick','idle')
        
        # __button_pack is a blue button sit on packer.
        # self.__button_shipped = RemoteVar_mqtt('twh/' + twh_id + '/packer/button/pack','idle')
        # self.__twh_packer = TwhRobot_Packer()
        mqtt_topic_of_ship = 'twh/' + wcs_instance_id + '/packer/button/pack'
        self.__shipper = Manual_Shipper(mqtt_topic_of_ship)
        self.__order_manager = Twh_OrderManager(wcs_instance_id)

        super().__init__(wcs_instance_id, deposit_queue, self.__order_manager)
        # self.__porters = list[Twh_LoopPorter]()
        # for i in range(4):
            # new_porter = Twh_LoopPorter(wcs_instance_id, i)
            # self.__porters.append(new_porter)

        # # __button_pick is a green button sit on packer.
        # self.__button_pick = RemoteVar_mqtt('twh/' + twh_id + '/packer/button/pick','idle')
        # # __button_pack is a blue button sit on packer.
        # self.__button_shipped = RemoteVar_mqtt('twh/' + twh_id + '/packer/button/pack','idle')
        # self.__packer = TwhRobot_Packer()
        # self.__shipper = TwhRobot_Shipper(button_shipped=self.__button_shipped)
        # self._wcs_state = 'idle'
        # self.__withdraw_orders_manager = WithdrawOrdersManager(twh_id, self.__packer, self.__shipper)
        self.__deposite_queue = deposit_queue

    def _start_deposit(self, new_deposit_request):
        Logger.Info(twh_factories[self._warehouse_id]['name'] + " -- Twh_WarehouseControlSystem::Do_deposit() ")
        Logger.Print("new_deposit_request", new_deposit_request)
        # the loop-porter will move to col-position
        row_id = int(new_deposit_request['row'])
        col_id = int(new_deposit_request['col'])
        layer_id = int(new_deposit_request['layer'])
        # Logger.Print("row_id", row_id)
        # Logger.Print("porters count", len(self.__porters))
        # porter = self.__porters[row_id]
        porter = g_workers[self._warehouse_id].loop_porters[row_id]
        self.__depositing_porter = porter
        Logger.Print('layer_id', layer_id)
        porter._MoveTo(col_id, layer_id)
        porter._show_layer_led()
        # Logger.Print("Twh_WarehouseControlSystem::Do_deposit()    point", 99)

    def _end_deposit(self):
        self.__depositing_porter._turn_off_leds()
 
