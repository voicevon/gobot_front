from von.mqtt.remote_var_mqtt import g_mqtt,g_mqtt_broker_config
from ocr_node_factory import OcrNodeFactory
import time

if __name__ == "__main__":
    
    time.sleep(10)
    g_mqtt_broker_config.client_id = "230510aaddf"
    g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)

    # ocr_node = CreateOcrNode('marker')
    # ocr_node = OcrNodeFactory.CreateOcrNode('def_areas', 'kvm_230508', 'windows_performance',is_new_kvm_node=False, is_new_app_window=False )
    # ocr_node = OcrNodeFactory.CreateOcrNode('all_in_one', 'kvm_230508','windows_performance', is_new_kvm_node= False, is_new_app_window=False)
    ocr_node = OcrNodeFactory.CreateOcrNode('demo_yjg', 'demo_yjg','temp_window', is_new_kvm_node= False, is_new_app_window=False)
    # ocr_node = OcrNodeFactory.CreateOcrNode('demo_fxm', 'demo_yjg','temp_window', is_new_kvm_node=True, is_new_app_window=False)
    # ocr_node = OcrNodeFactory.CreateOcrNode('view_demo', 'demo_yjg','temp_window', is_new_kvm_node=False, is_new_app_window=False)

    while True:
        ocr_node.SpinOnce()