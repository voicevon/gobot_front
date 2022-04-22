from von.mqtt_helper import g_mqtt, MQTT_ConnectionConfig
import base64

def on_received(topic, payload):
    if topic == 'gobot/head/eye/origin':
        image_json = open('origin.jpg', 'wb')
        image_json.write(payload) 
        image_json.close() 
        print('==================================================')

config = MQTT_ConnectionConfig()
config.broker="voicevon.vicp.io"
config.port = 1883
config.client_id = "Y22-0422"
config.uid = 'von'
config.password = 'von1970'
g_mqtt.connect_to_broker(config)
g_mqtt.append_on_message_callback(on_received)
g_mqtt.subscribe('gobot/head/eye/origin')
# g_mqtt.subscribe('gobot/smf/current')
while True:
    pass