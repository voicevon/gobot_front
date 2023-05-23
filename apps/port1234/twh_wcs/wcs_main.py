from twh_database.bolt_nut import twh_factories


from twh_wcs.wcs_deck_factory import DeckGroupFactory
from twh_wcs.wcs_workers_factory import WorkersFactory
from twh_wcs.wcs_pastor_factory import PastorFactory, g_pastors
from twh_wcs.wcs_component_factory import ComponentFactory

from twh_wcs.von.wcs.gcode_sender import g_gcode_senders
from von.mqtt.mqtt_agent import g_mqtt,g_mqtt_broker_config
from von.logger import Logger
import multiprocessing
import time


def WCS_Main(deposit_queue:multiprocessing.Queue):
        Logger.Info("WCS_Main() is starting  on my own process.")
        g_mqtt.connect_to_broker(g_mqtt_broker_config,blocked_connection=True)                # DebugMode, must be turned off.  
        
        # Create all instance and save it in g_wcss
        for warehouse_id in list(twh_factories.keys()):
            PastorFactory.Create_Pastor(warehouse_id, deposit_queue)
            DeckGroupFactory.CreateDeckGroups(warehouse_id)
            ComponentFactory.CreateComponents(warehouse_id)
            WorkersFactory.Create_WcsWorkers(warehouse_id)
            Logger.Info("Twh_Wcs_Main()   Created wcs_unit----->"  + warehouse_id)

        while True:
            PastorFactory.EachPastor_SpinOnce()
            WorkersFactory.EachWorker_SpinOnce()
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


