from von.amq_mqtt_bridges import AmqMqtt_Bridges
from von.amq_agent import g_amq, g_amq_broke_config
from von.mqtt_agent import g_mqtt, g_mqtt_broker_config


if __name__ == '__main__':
    g_amq.connect_to_broker(g_amq_broke_config)
    g_mqtt.connect_to_broker(g_mqtt_broker_config)
    bridges = AmqMqtt_Bridges()

    bridges.Append('twh_221109_gcode')
    while True:
        g_amq.process_data_events(0.1)
        bridges.spin_once()
