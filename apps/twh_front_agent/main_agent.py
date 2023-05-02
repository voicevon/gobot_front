
from smart_kvm import SmartKvm, SmartKvmConfig, SmartKvmTargetArea
from von.remote_var_mqtt import RemoteVar_mqtt, g_mqtt, g_mqtt_broker_config
from von.logger import Logger

def SetTargetArea(area_name) -> SmartKvmTargetArea:
    if area_name == 'test':
        test_area = SmartKvmTargetArea()
        test_area.name = area_name
        test_area.mark_filename = "abc.png"
        test_area.height = 100
        test_area.width = 50
        test_area.left_offset = -100
        test_area.top_offset = 30
        return test_area
    else:
        Logger.Error('SetTargetArea()')
        Logger.Print('request area_name', area_name)
        return None # type: ignore


if __name__ == "__main__":
    g_mqtt_broker_config.client_id = "230502"
    g_mqtt.connect_to_broker(g_mqtt_broker_config)

    config = SmartKvmConfig()

    config.target_areas.append(SetTargetArea('test'))

    smart_kvm = SmartKvm(0, config)

        

