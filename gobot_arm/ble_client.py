from bluepy import btle
from bluepy import Scanner

class ble():
    def __init__(self):
        self.__server_mac = 'b4:e6:2d:b2:f8:8f'

    def scan(self):
        scanner = bluepy.Scanner()
        devices = scanner.scan(timeout = 3)
        print('Found %d devices in %d seconds' % (len(devices), timeout))
        for dev in devices:
            print('Name', dev.getValueText(9))
            print('Address:', dev.addr)

    def connect_to_server(self):
        pass

    def list_services_on_server(self):
        dev = btle.Peripheral(self.__server_mac)
        for svc in dev.services:
            print(str(svc))

 
if __name__ =='__main__':
    print('practice BLE')
    runner = ble()
    runner.scan()




#print ("Connecting...")
#dev = btle.Peripheral("B0:B4:48:BF:C9:83")
#dev =  btle.Peripheral('b4:e6:2d:b2:f8:8f')

#print ("Services...")

#for svc in dev.services:
#    print (str(svc))

lightSensor = btle.UUID("f000aa70-0451-4000-b000-000000000000")
 
lightService = dev.getServiceByUUID(lightSensor)
for ch in lightService.getCharacteristics():
     print (str(ch))

import time
import binascii
uuidConfig = btle.UUID("f000aa72-0451-4000-b000-000000000000")
lightSensorConfig = lightService.getCharacteristics(uuidConfig)[0]
# Enable the sensor
lightSensorConfig.write(bytes("\x01"))

time.sleep(1.0) # Allow sensor to stabilise
 
uuidValue  = btle.UUID("f000aa71-0451-4000-b000-000000000000")
lightSensorValue = lightService.getCharacteristics(uuidValue)[0]
# Read the sensor
val = lightSensorValue.read()
print ("Light sensor raw value", binascii.b2a_hex(val))




