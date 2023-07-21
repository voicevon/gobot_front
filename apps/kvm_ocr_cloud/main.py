from von.mqtt.remote_var_mqtt import g_mqtt,g_mqtt_broker_config
from ocr_node_factory import OcrNodeFactory
import time




# who_am_i = "auto runner on rasp-pi"
who_am_i = "normal"







if __name__ == "__main__":
        if who_am_i == "auto runner on rasp-pi":
            time.sleep(10)  # In case of running from rc.local,   mqtt-connection must be after network is connected.

        g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)
        # ocr_node = CreateOcrNode('marker')
        # ocr_node = OcrNodeFactory.CreateOcrNode('def_areas', 'kvm_230508', 'windows_performance',is_new_kvm_node=False, is_new_app_window=False )
        # ocr_node = OcrNodeFactory.CreateOcrNode('all_in_one', 'kvm_230508','windows_performance', is_new_kvm_node= False, is_new_app_window=False)
        # ocr_node = OcrNodeFactory.CreateOcrNode('demo_yjg', 'demo_yjg','temp_window', is_new_kvm_node= False, is_new_app_window=False)
        # ocr_node = OcrNodeFactory.CreateOcrNode('demo_fxm', 'demo_yjg','temp_window', is_new_kvm_node=True, is_new_app_window=False)
        # ocr_node = OcrNodeFactory.CreateOcrNode('view_demo', 'demo_yjg','temp_window', is_new_kvm_node=False, is_new_app_window=False)
        # ocr_node = OcrNodeFactory.CreateOcrNode('yalefu', 'yalefu_screen','none', is_new_kvm_node=False, is_new_app_window=False)
        ocr_node = OcrNodeFactory.CreateOcrNode('yalefu_viewer', 'yalefu_screen','none', is_new_kvm_node=False, is_new_app_window=False)

        while True:
            ocr_node.SpinOnce()