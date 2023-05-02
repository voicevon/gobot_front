
from window_ocr import WindowOcr, OcrUnits_Manager, OcrUnit
from von.remote_var_mqtt import RemoteVar_mqtt, g_mqtt, g_mqtt_broker_config
from von.logger import Logger

def CreateOcrUnit(unit_name) -> OcrUnit:
    if unit_name == 'youtube_title':
        unit = OcrUnit()
        unit.name = unit_name
        unit.template_filename = "template_images/youtube.png"
        unit.height = 660
        unit.width = 450
        unit.left_offset = -20
        unit.top_offset = 50
        return unit
    else:
        Logger.Error('SetTargetArea()')
        Logger.Print('request unit_name', unit_name)
        return None # type: ignore


if __name__ == "__main__":
    # g_mqtt_broker_config.client_id = "230502"
    # g_mqtt.connect_to_broker(g_mqtt_broker_config)

    units = OcrUnits_Manager()
    units.Append_Unit(CreateOcrUnit('youtube_title'))
    window_ocr = WindowOcr(99, units)
    while True:
        window_ocr.SpinOnce()

        

