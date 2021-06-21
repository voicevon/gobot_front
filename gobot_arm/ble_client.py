from bluepy import btle
from bluepy.btle import Scanner
from bluepy.btle import Peripheral, DefaultDelegate
import time

class MyDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleNotification(self, cHandle, data):
        print('Notification is invoked')

class ble():
    def __init__(self):
        self.__server_mac = 'b4:e6:2d:b2:f8:8f'
        self.dev = btle.Peripheral(self.__server_mac)

    def scan(self):
        scanner = Scanner()
        devices = scanner.scan(timeout = 3)
        print('Scan result:  Found %d devices in %d seconds------------' % (len(devices), 3))
        for dev in devices:
            print('Name', dev.getValueText(9))
            print('Address:', dev.addr)

    def list_services_on_server(self):
        print('Services on server  ------------------')
        for svc in self.dev.services:
            print(str(svc))

    

    def connect_to_server(self):
        self.dev.withDelegate(MyDelegate())
        svc = self.dev.getServiceByUUID('4fafc201-1fb5-459e-8fcc-c5c9c331914b')
        self.arm_info = svc.getCharacteristics('beb5483e-36e1-4688-b7f5-ea07361b26a8')[0]
        self.house_info = svc.getCharacteristics('beb5483e-36e1-4688-b7f5-ea07361b26a8')[1]




    def update_charactoreristic(self, channel, new_value):
        if channel == 0:
            ch = self.arm_info
        if channel == 1:
            ch = self.house_info
        
        ch.write(bytes(new_value))
    
    def spin(self):
        while True:
            xx = self.arm_info.read()
            print('arm info: ', xx)
            yy = self.house_info.read()
            print('house info: ',yy)
            time.sleep(1.7)
            pass

if __name__ =='__main__':
    print('practice BLE')
    runner = ble()
    runner.scan()
    runner.list_services_on_server()
    runner.connect_to_server()
    runner.spin()






