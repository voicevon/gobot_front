from bluepy import btle  
'''
sudo apt-get install libglib2.0-dev
pip3 install bluepy
sudo blescan # to verify bluepy
'''
from bluepy.btle import Scanner
from bluepy.btle import Peripheral, DefaultDelegate
'''
Alternative lib is bleak
https://github.com/hbldh/bleak
'''
import time
import logging

from enum import Enum



# Instruction:
#    sudo btmgmt le on
#    sudo hciconfig hci0 reset
#    sudo python3 ble_client.py
#
#

# https://anthonychiu.xyz/2016/04/05/communication-between-raspberry-pi-and-multiple-arduinos-via-bluetooth-low-power-ble/

class MyDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print ('Disovered device',dev.addr)
        if isNewData:
            print('Received new data from ', dev.addr)

    def handleNotification(self, cHandle, data):
        print('Notification is invoked')

class BleConnState(Enum):
    CORVERD = 0
    DISCOVERED =1
    CONNECTING = 2
    CONNECTED = 3
    DISCONNECTED =4

class BleConnection():
    '''
.                 Disconnected <---<---<---<---<---<----
.                     |     \           ^                ^
.                     |   Connect()     |                |
.                     |                 |                |
.    Covered --->  Discovered  ---> Connecting --->  Connected  ---> GATT ??
.      \              \         \                        \
.       \              \       Connect()                  \
.        \              \                                  \----- Read, Write, Notify, Indicate.
.         \              \------- Got Server Mac Address
.          \              \------ Created device, Service, Characteristics,
.           \              \----- 
.            \                          
.             \--- Don't know Server mac address.

    '''

    COVERED = 0
    DISCONNECTED = 0

    def __init__(self, server_name):
        self.server_name = server_name
        self.__server_mac_addr = None
        self.state = BleConnState.CORVERD
        self.dev = None
        '''
        0 = Covered
        1 = Discovered
        2 = Connecting
        3 = Connected
        4 = Disconnected
        '''
    def Disconnect(self):
        self.dev.disconnect()
        self.state = BleConnState.DISCONNECTED

    def onDisconnect(self):
        self.state =  BleConnState.DISCONNECTED

    # def SetMacAddr(self, server_mac_addr):
    #     self.__server_mac_addr = server_mac_addr
    #     self.state = BleConnState.DISCOVERED

    def Connect(self):
        if self.state == BleConnState.DISCONNECTED or \
            self.state == BleConnState.DISCOVERED:
            try:
                self.dev = btle.Peripheral(self.__server_mac_addr)
                # print('           Services:')
                # for svc in self.__dev.services:
                #     print('                ', str(svc))
                self.dev.withDelegate(MyDelegate())
                self.state = BleConnState.CONNECTED
            except:
                logging.error('Connect to BLEServer,  got exception!\n')
                # self.state = BleConnState.DISCONNECTED
        elif self.state == BleConnState.CORVERD:
            self.Scan()

    def Scan(self):
        scanner = Scanner()
        devices = scanner.scan(timeout = 8)
        for dev in devices:
            name = dev.getValueText(9)
            print('-----------------Scanning nearby devices..............')
            print(name, self.server_name)
            # if name == 'ConInt-Arm-' + self.__gobot_id:
            if name == self.server_name:
                logging.info('---------------------------------------------')
                logging.info('      Discoverd target !')
                self.__server_mac_addr = dev.addr
                self.state = BleConnState.DISCOVERED
                # self.__arm_conn.SetMacAddr(dev.addr)
                return
    # def connect_to_arm(self, server_mac_addr):
    #     logging.info('---------------------------------------------')
    #     logging.info('      Discoverd Arm !')
    #     try:
    #         self.__dev_arm = btle.Peripheral(server_mac_addr)
    #         self.__dev_arm.withDelegate(MyDelegate())
    #         svc = self.__dev_arm.getServiceByUUID(self.__ARM_SERVICE_UUID)
    #         self.arm_state = svc.getCharacteristics(self.__ARM_STATE_UUID)[0]
    #         self.arm_action = svc.getCharacteristics(self.__ARM_ACTION_UUID)[0]
    #         logging.info('      BLE connected to GATT server Arm !\n')
    #         self.is_connected_arm = True
    #     except:
    #         logging.error('******************', 'connect to Arm got exception!\n')

#scanner = Scanner().withDelegate(MyDelegate())
#devices = scanner.scan(10.0)
#for dev in devices:
#    print('Devide %s (%s), RSSI=%d DB' %(dev.addr, dev.addrType, dev.rssi))
#    for(adtype, desc, value) in dev.getScanData():
#       print('%s = %s'%(desc, value))

class BleClient():
    def __init__(self,gobot_id):
        self.__gobot_id = gobot_id
        self.__arm_conn = BleConnection("ConInt-Arm-" + gobot_id)
        self.__house_conn = BleConnection("ConInt-House-" + gobot_id)
        # self.is_connected_arm = False
        # self.is_connected_house = False


        self.__ARM_SERVICE_UUID = "d592c9aa-0594-11ec-9a03-0242ac130003"
        self.__ARM_STATE_UUID  = "b7c65186-0610-11ec-9a03-0242ac130003"
        self.__ARM_ACTION_UUID = "c21a1596-0610-11ec-9a03-0242ac130003"

        self.__HOUSE_SERVICE_UUID = "b416890c-062e-11ec-9a03-0242ac130003"
        self.__HOUSE_STATE_UUID = "bfa35098-062e-11ec-9a03-0242ac130003"
        self.__HOUSE_ACTION_UUID = "c52ca230-062e-11ec-9a03-0242ac130003"

    def scan_arm_house(self):
        scanner = Scanner()
        devices = scanner.scan(timeout = 8)
        print('Scan result:  Found %d devices in %d seconds------------' % (len(devices), 3))
        for dev in devices:
            name = dev.getValueText(9)
            print('\n        Server Name:', name)
            print('        Mac Address:', dev.addr)

            if name == 'ConInt-Arm-' + self.__gobot_id:
                logging.info('---------------------------------------------')
                logging.info('      Discoverd Arm !')
                # self.connect_to_arm(dev.addr)
                self.__arm_conn.SetMacAddr(dev.addr)

            if name == 'ConInt-House-' + self.__gobot_id:
                self.__house_conn.SetMacAddr(dev.addr)
                # self.connect_to_house(dev.addr)

    def list_services_on_server(self, server_mac):
        logging.info('Services on server  ------------------')
        self.dev = btle.Peripheral(server_mac)
        for svc in self.dev.services:
            print(str(svc))

    def connect_to_arm(self):
        self.__arm_conn.Connect()
        if self.__arm_conn.state == BleConnState.CONNECTED:
            svc = self.__arm_conn.dev.getServiceByUUID(self.__ARM_SERVICE_UUID)
            self.arm_state = svc.getCharacteristics(self.__ARM_STATE_UUID)[0]
            self.arm_action = svc.getCharacteristics(self.__ARM_ACTION_UUID)[0]
            logging.info('      BLE connected to GATT server Arm !\n')




        # logging.info('---------------------------------------------')
        # logging.info('      Discoverd Arm !')
        # try:
        #     self.__dev_arm = btle.Peripheral(server_mac_addr)
        #     self.__dev_arm.withDelegate(MyDelegate())
        #     svc = self.__dev_arm.getServiceByUUID(self.__ARM_SERVICE_UUID)
        #     self.arm_state = svc.getCharacteristics(self.__ARM_STATE_UUID)[0]
        #     self.arm_action = svc.getCharacteristics(self.__ARM_ACTION_UUID)[0]
        #     logging.info('      BLE connected to GATT server Arm !\n')
        #     self.is_connected_arm = True
        # except:
        #     logging.error('******************', 'connect to Arm got exception!\n')

    def connect_to_house(self, server_mac_addr):
        logging.info('---------------------------------------------')
        logging.info('      Discovered House !')
        try:
            self.__dev_house = btle.Peripheral(server_mac_addr)
            self.__dev_house.withDelegate(MyDelegate())
            svc = self.__dev_house.getServiceByUUID(self.__HOUSE_SERVICE_UUID)
            self.house_state = svc.getCharacteristics(self.__HOUSE_STATE_UUID)[0]
            self.house_action = svc.getCharacteristics(self.__HOUSE_ACTION_UUID)[0]
            logging.info('      BLE connected to GATT server House !\n')
            self.is_connected_house = True
        except:
            logging.error('******************', 'connect to House got exception!')

    def write_characteristic(self, new_value):
        try:
            self.arm_action.write(bytes(new_value))
            logging.info('Updated charactoristic')
        #except bluepy.btle.BTLEDisconnectError:
        except:
            pass
            #self.dev.connect(self.__server_mac)
            # print('ble_Write() Disconnected --> reconnecting')
            # self.list_services_on_server()
            # self.connect_to_server()
            #print('ble_write() Disconnected --> reconnecting  ')

    def read_characteristic(self):
        try:
            action_code = self.arm_info.read()
            return action_code
        #except bluepy.btle.BTLEDisconnectError:
        except:
            print("ble_read() Device disconnected-- reconnecting")
            self.list_services_on_server()
            self.connect_to_server()
            #continue
            action_code = self.arm_info.read()
            return action_code
        
    def spin_once(self):
        if self.__arm_conn.state == BleConnState.CONNECTED:
            received = self.arm_info.read()
            logging.info('arm info: %s', received)
        else:
            self.connect_to_arm()

        # if self.__house_conn.state == CONNECTED:
        #     pass
        # else:
        #     self.__house_conn.Connect()

    # def disconnect(self):
    #     self.__dev_arm.disconnect()
    #     self.__dev_house.disconnect()
    #     time.sleep(1)

g_bleClient = BleClient('213401')
# def signal_handler(sig, frame):
#     print("You pressed Ctrl+C")
#     g_bleClient.disconnect()
#     sys.exit(0)

# import signal        
# signal.signal(signal.SIGINT, signal_handler)


if __name__ == '__main__':
    import sys
    logging.basicConfig(level=logging.DEBUG)
    logging.info('@@@@@@@@@@@@@@@@@@@@@@@@@')
    # g_bleClient.scan_arm_house()
    while True:
        g_bleClient.spin_once()
    # runner = BleClient()


    i = 65

    while True:
        time.sleep(1.7)
        i += 1
        if i> 128:
            i = 32
        g_bleClient.write_characteristic([i])
