# from von.ocr.front_cloud.ocr_factory import OcrFactory

from von.ocr.ocr_factory import OcrFactory
from von.mqtt.mqtt_agent import g_mqtt, g_mqtt_broker_config
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
import json

def CreateNodeConfiguration(node_name):

    node_config = OcrFactory.CreateKvmNodeConfig(node_name)
    payload = json.dumps(node_config)

    mqtt_config = RemoteVar_mqtt(node_config['my_topic'], payload)
    while not  mqtt_config.rx_buffer_has_been_updated():
        pass

g_mqtt_broker_config.client_id = "250506c"
g_mqtt.connect_to_broker(g_mqtt_broker_config)

CreateNodeConfiguration("kvm_230506")





