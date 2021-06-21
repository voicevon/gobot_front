from bluepy import btle
from bluepy.btle import Scanner
from bluepy.btle import Peripheral, DefaultDelegate

class MyDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleNotification(self, cHandle, data):
        print('Notification is invoked')

class ble():
    def __init__(self):
        self.__server_mac = 'b4:e6:2d:b2:f8:8f'
        #:w
        #self.__n

    def scan(self):
        scanner = Scanner()
        devices = scanner.scan(timeout = 3)
        print('Scan result:  Found %d devices in %d seconds------------' % (len(devices), 3))
        for dev in devices:
            print('Name', dev.getValueText(9))
            print('Address:', dev.addr)

    def list_services_on_server(self):
        self.dev = btle.Peripheral(self.__server_mac)
        print('Services on server  ------------------')
        for svc in self.dev.services:
            print(str(svc))

    

    def connect_to_server(self):
        print('Connect to server--------------------' )
        #        self.p = Peripheral(self.__server_mac, addrType=ADDR_TYPE_PUBLIC)
        #self.p = Peripheral(self.__server_mac)
        self.dev.setDelegate(MyDelegate())
        svc = self.dev.getServicesByUUID('4fafc201-1fb5-459e-8fcc-c5c9c331914b')
        ch = svc.getCharacteristics('')
        print('Connected') 



    def update_charactoreristic(self, new_value):
        pass
    
    def spin(self):
        while True:
            pass

if __name__ =='__main__':
    print('practice BLE')
    runner = ble()
    runner.scan()
    runner.list_services_on_server()
    runner.connect_to_server()
    runner.spin()




#print ("Connecting...")
#dev = btle.Peripheral("B0:B4:48:BF:C9:83")
#dev =  btle.Peripheral('b4:e6:2d:b2:f8:8f')

#print ("Services...")

#for svc in dev.services:
#    print (str(svc))
if False:
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




