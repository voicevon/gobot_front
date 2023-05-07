import cv2
from von.mqtt.mqtt_agent import g_mqtt, g_mqtt_broker_config
from von.logger import Logger
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
import time
import json
from libs.kvm_node import KvmNode




# TODO:   config this in outside of code.  like .INI file.
my_os = 'Windows'
# my_os = 'Pi_lite'
node_name = 'kvm_230506'

if __name__ == '__main__':
    g_mqtt_broker_config.client_id = node_name
    g_mqtt.connect_to_broker(g_mqtt_broker_config)
    Logger.Info("Start to load configuration")
    Logger.Print('my node_name', node_name)

    mqtt_topic_of_node_config = 'ocr/kvm/' + node_name + '/config'
    config_getter = RemoteVar_mqtt(mqtt_topic_of_node_config, None)
    while not config_getter.rx_buffer_has_been_updated():
        pass
    config_json =  json.loads(config_getter.get())
    kvm_node = KvmNode(my_os,config_json)


    while True:
        image = kvm_node.Capture_Camera()
        kvm_node.publish(image)


