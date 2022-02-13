from von.mqtt_helper import g_mqtt
import base64

def on_received(topic, payload):
    if topic == 'gobot/head/eye/origin':
        image_json = open('origin.jpg', 'wb')
        image_json.write(payload) 
        image_json.close() 
        print('==================================================')

g_mqtt.connect_to_broker('T220213b','voicevon.vicp.io',1883,'von','von1970')
g_mqtt.append_on_message_callback(on_received)
g_mqtt.subscribe('gobot/head/eye/origin')
# g_mqtt.subscribe('gobot/smf/current')
while True:
    pass