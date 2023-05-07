from von.mqtt.mqtt_agent import g_mqtt, g_mqtt_broker_config
from von.logger import Logger
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
from libs.kvm_node_camera import KvmNodeCamera


# my_os = 'Windows'
my_os = 'Pi_lite'
node_name = 'kvm_230506'

if __name__ == '__main__':
    g_mqtt_broker_config.client_id = node_name
    g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)
    mqtt_topic_of_node_config = 'ocr/kvm/' + node_name + '/config'

    Logger.Info("Start to load configuration")
    Logger.Print('my node_name', node_name)
    Logger.Print('mqtt_topic_of_node_config', mqtt_topic_of_node_config)

    config_getter = RemoteVar_mqtt(mqtt_topic_of_node_config, None, for_loading_config=True)
    kvm_node = KvmNodeCamera(my_os,config_getter)


    while True:
        image = kvm_node.Capture_Image()
        kvm_node.publish(image)


