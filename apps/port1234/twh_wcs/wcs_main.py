from twh_wcs.twhwcs_loop_manual_pack_system.twhwcs_loop_manual_packer import TwhWcs_LoopManualPacker
from twh_wcs.twhwcs_loop_tube_system.twhwcs_loop_tube import Twh_LoopTubeSystem

from twh_database.bolt_nut import twh_factories
from twh_wcs.von.wcs.wcs_system_base import Wcs_SystemBase

from twh_wcs.wcs_instance import g_wcss, WcsInstance, CreateWcsInstance

# from twh_wcs.wcs_instance import g_wcs_hardwares, WcsUnit_Hardware
from twh_wcs.von.wcs.gcode_sender import g_gcode_senders
from von.mqtt.mqtt_agent import g_mqtt,g_mqtt_broker_config
from von.logger import Logger

import multiprocessing
import time


def WCS_Main(deposit_queue:multiprocessing.Queue):
        Logger.Info("WCS_Main() is starting  on my own process.")
        g_mqtt.connect_to_broker(g_mqtt_broker_config,blocked_connection=True)                # DebugMode, must be turned off.  

        for wcs_instance_id in list(twh_factories.keys()):
            wcs_instance = CreateWcsInstance(wcs_instance_id)
            wcs_software = Twh_LoopTubeSystem(wcs_instance_id, deposit_queue)
            wcs_instance.software = wcs_software
            Logger.Info("Twh_Wcs_Main()   Created wcs_unit----->"  + wcs_instance_id)

        while True:
            for k,v in g_wcss.items():
                v.software.SpinOnce()
                
                # wcs_unit.SpinOnce()

            for gcode_sender in g_gcode_senders:
                gcode_sender.SpinOnce()

            time.sleep(0.5)


# from wms to wcs
wcs_deposit_queue = multiprocessing.Queue()         

def Start_TwhWcs_Process():
    
    p = multiprocessing.Process(target=WCS_Main, args=(wcs_deposit_queue,))
    p.start() 
    # Logger.Info('WCS is running on new process.....')


    # https://pymotw.com/2/multiprocessing/communication.html#communication-between-processes


