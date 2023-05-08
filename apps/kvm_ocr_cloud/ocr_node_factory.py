from ocr_node import OcrNode
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
import json

class OcrNodeFactory:
    
    @classmethod
    def CreateOcrNode(cls, id:str, kvm_node_name:str, app_window_name:str, is_new_kvm_node = False):
        router = {}
        router['my_os'] = 'windows'
        router['from_screen_capture'] = False
        router['from_camera_capture'] = False
        router['from_mqtt'] = False
        router['screen_image_to_mqtt'] = False
        router['screen_image_to_app_window_identifier'] = False
        router['screen_image_to_image_divider'] = False
        router['screen_image_to_ocr'] = False
        router['screen_string_to_matt'] = False
        router['screen_image_to_tool'] = False
        router['small_images_to_mqtt'] = False
        router['small_images_to_ocr'] = False
        router['small_strings_to_mqtt'] = False


        if id == 'marker':
            # template as marker
            router['kvm_node_name'] = kvm_node_name
            router['app_window_name'] = app_window_name
            router['from_mqtt'] = True
            router['screen_image_to_tool'] = True

        if id == 'areas':
            # tool:  calibrate areas for divider.
            router['kvm_node_name'] = kvm_node_name
            router['app_window_name'] = app_window_name
            router['from_mqtt'] = True
            router['screen_image_to_areas_marker'] = True

        if id == 'recorder':
            # cloud-edge. video recoder
            router['from_kvm_nodes'] = {'kvm_230506','kvm_230508'}
            router['kvm_node_name'] = 'fake_kvm_node'
            router['app_window_name'] = 'fake_app_window'
            router['from_mqtt'] = True
            router['screen_image_to_tool'] = True


        if id == '101':
            # hard capture, on rasperberry pi.
            router['kvm_node_name'] = 'kvm_230506'
            router['app_window_name'] = 'ubuntu_performance'
            router['my_os'] = 'Pi_lite'
            router['from_camera_capture'] = True
            router['screen_image_to_mqtt'] = True
        
        if id == '102':
            # soft capture, on window
            router['kvm_node_name'] = 'kvm_230508'
            router['app_window_name'] = 'windows_performance'
            router['from_screen_capture'] = True
            router['screen_image_to_mqtt'] = True
            router['screen_image_to_ocr'] = True

        if is_new_kvm_node:
            mqtt_topic_of_kvm_node_config = 'ocr/kvm/' + router['kvm_node_name'] + '/config'
            node_config ={}
            node_config["node_name"] = router['kvm_node_name']
            node_config["fps"] = 1
            node_config['resolution'] = {'left': 0, 'top': 0, 'width': 1366, 'height': 768}
            _ = RemoteVar_mqtt(mqtt_topic_of_kvm_node_config, json.dumps(node_config))

        ocr_node = OcrNode(router) 
        return ocr_node