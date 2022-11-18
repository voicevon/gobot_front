from von.mqtt_agent import g_mqtt_broker_config, g_mqtt
import base64

def on_received(topic, payload):
    if topic == 'gobot/head/eye/origin':
        image_json = open('origin.jpg', 'wb')
        image_json.write(payload) 
        image_json.close() 
        print('==================================================')


g_mqtt.connect_to_broker(g_mqtt_broker_config)
g_mqtt.append_on_received_message_callback(on_received)
g_mqtt.subscribe('gobot/head/eye/origin')
# g_mqtt.subscribe('gobot/smf/current')
while True:
    pass