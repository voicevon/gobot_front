from von.mqtt.remote_var_mqtt import g_mqtt,g_mqtt_broker_config
from ocr_node_factory import OcrNodeFactory


if __name__ == "__main__":

    g_mqtt_broker_config.client_id = "230502cfg"
    g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)

    # ocr_node = CreateOcrNode('marker')
    ocr_node = OcrNodeFactory.CreateOcrNode('def_areas', 'kvm_230508', 'windows_performance',is_new_kvm_node=False, is_new_app_window=True )
    # ocr_node = OcrNodeFactory.CreateOcrNode('102',True)

    while True:
        ocr_node.SpinOnce()