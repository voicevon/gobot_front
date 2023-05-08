from ocr_node import OcrNode
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
import json

class OcrNodeFactory:
    
    @classmethod
    def CreateKvmNodeConfig(cls, routing):
            kvm_node_config ={}
            kvm_node_config["node_name"] = routing['kvm_node_name']
            kvm_node_config["fps"] = 1
            kvm_node_config['resolution'] = {'left': 0, 'top': 0, 'width': 1366, 'height': 768}

            mqtt_topic_of_kvm_node_config = 'ocr/kvm/' + routing['kvm_node_name'] + '/config'
            _ = RemoteVar_mqtt(mqtt_topic_of_kvm_node_config, json.dumps(kvm_node_config))

    @classmethod
    def CreateAppWindowConfig(cls, routing):
            app_window_config = {}
            app_window_config['name'] = routing['app_window_name']


            all_areas = []
            for i in range(10):
                first_area = {}
                first_area['x1'] = 1
                first_area['y1'] = 2
                first_area['x2'] = 3
                first_area['y2'] = 4
                all_areas.append(first_area)
            app_window_config["areas"] = all_areas

            mqtt_topic_of_app_window_config = 'ocr/' + routing['app_window_name'] + '/config'
            _ = RemoteVar_mqtt(mqtt_topic_of_app_window_config, json.dumps(app_window_config))

    @classmethod
    def CreateOcrNode(cls, id:str, kvm_node_name:str, app_window_name:str, is_new_kvm_node = False, is_new_app_window = False):
        routing = {}
        routing['my_os'] = 'windows'
        routing['from_screen_capture'] = False
        routing['from_camera_capture'] = False
        routing['from_mqtt'] = False
        routing['screen_image_to_mqtt'] = False
        routing['screen_image_to_app_window_identifier'] = False
        routing['screen_image_to_image_divider'] = False
        routing['screen_image_to_ocr'] = False
        routing['screen_string_to_matt'] = False
        routing['screen_image_to_tool'] = False
        routing['small_images_to_mqtt'] = False
        routing['small_images_to_ocr'] = False
        routing['small_strings_to_mqtt'] = False


        if id == 'marker':
            # template as marker
            routing['kvm_node_name'] = kvm_node_name
            routing['app_window_name'] = app_window_name
            routing['from_mqtt'] = True
            routing['screen_image_to_tool'] = True

        if id == 'def_areas':
            # tool:  calibrate areas for divider.
            routing['kvm_node_name'] = kvm_node_name
            routing['app_window_name'] = app_window_name
            routing['from_mqtt'] = True
            routing['screen_image_to_tool_areas_marker'] = True

        if id == 'recorder':
            # cloud-edge. video recoder
            routing['from_kvm_nodes'] = {'kvm_230506','kvm_230508'}
            routing['kvm_node_name'] = 'fake_kvm_node'
            routing['app_window_name'] = 'fake_app_window'
            routing['from_mqtt'] = True
            routing['screen_image_to_tool'] = True


        if id == '101':
            # hard capture, on rasperberry pi.
            routing['kvm_node_name'] = 'kvm_230506'
            routing['app_window_name'] = 'ubuntu_performance'
            routing['my_os'] = 'Pi_lite'
            routing['from_camera_capture'] = True
            routing['screen_image_to_mqtt'] = True
        
        if id == '102':
            # soft capture, on window
            routing['kvm_node_name'] = 'kvm_230508'
            routing['app_window_name'] = 'windows_performance'
            routing['from_screen_capture'] = True
            routing['screen_image_to_mqtt'] = True
            routing['screen_image_to_ocr'] = True

        if is_new_kvm_node:
            cls.CreateKvmNodeConfig(routing)

        if is_new_app_window:
            cls.CreateAppWindowConfig(routing)


        ocr_node = OcrNode(routing) 
        return ocr_node