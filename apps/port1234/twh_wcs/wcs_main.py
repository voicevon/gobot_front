from twh_database.bolt_nut import twh_factories

from twh_wcs.wcs_workers_factory import WorkerFactory
from twh_wcs.wcs_warehouse_factory import WarehouseFactory, g_warehouses

from twh_wcs.von.wcs.gcode_sender import g_gcode_senders
from von.mqtt.mqtt_agent import g_mqtt,g_mqtt_broker_config
from von.logger import Logger
import multiprocessing
import time


def WCS_Main(deposit_queue:multiprocessing.Queue):
        Logger.Info("WCS_Main() is starting  on my own process.")
        g_mqtt.connect_to_broker(g_mqtt_broker_config,blocked_connection=True)                # DebugMode, must be turned off.  
        
        # Create all instance and save it in g_wcss
        for wcs_instance_id in list(twh_factories.keys()):
            WorkerFactory.Create_WcsWorkers(wcs_instance_id)
            WarehouseFactory.Create_Warehouse(wcs_instance_id, deposit_queue)
            Logger.Info("Twh_Wcs_Main()   Created wcs_unit----->"  + wcs_instance_id)

        while True:
            for id, leader in g_warehouses.items():
                leader.SpinOnce()
            # ????
            # for id, worker in g_workers.items():
            #      worker.SpinOnce() 

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


