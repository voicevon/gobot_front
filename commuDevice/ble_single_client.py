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


class BleServerHead:
        AxisName = ''
        BleDeviceName = ''
        BleServiceUUID = ''
        BleCommuUUID = ''
        BleStateUUID = ''


class BleConnection():
    '''
    .                 Disconnected <---<---<---<---<---<----
    .                     |     \           ^                ^
    .                     |   Connect()     |                |
    .                     |                 |                |
    .    Covered --->  Discovered  ---> Connecting --->  Connected  --->  Access GATT 
    .      \      \       \         \                        \
    .       \      \       \       Connect()                  \
    .        \    Scan()    \                                  \----- Read, Write, Notify, Indicate.
    .         \              \------- Got Server Mac Address
    .          \              \------ Created device, Service, Characteristics,
    .           \              \----- 
    .            \                          
    .             \--- Don't know Server mac address.

    '''

    COVERED = 0
    DISCONNECTED = 0

    def __init__(self, server_head:BleServerHead):
        self.__server_head = server_head
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
                logging.error(' %s  Connect to BLEServer,  got exception!\n ' % self.__server_head.BleDeviceName)
        elif self.state == BleConnState.CORVERD:
            self.Scan()


        
    def ResetConnection(self):
        self.state = BleConnState.DISCONNECTED

    def Scan(self):
        scanner = Scanner()
        devices = scanner.scan(timeout = 8)
        for dev in devices:
            name = dev.getValueText(9)
            print('-----------------Scanning nearby devices..............')
            print(name, self.__server_head.BleDeviceName)
            # if name == 'ConInt-Arm-' + self.__gobot_id:
            if name == self.__server_head.BleDeviceName:
                logging.info('---------------------------------------------')
                logging.info('      Discoverd target !')
                self.__server_mac_addr = dev.addr
                self.state = BleConnState.DISCOVERED
                return


class BleSingleClient():
    def __init__(self, server:BleServerHead):
        self.__server = server
        self.__connection = BleConnection(server.name)
        self.__char_state = []
        self.__char_commu = []

    def connect_to_server(self):
        self.__connection.Connect()
        if self.__connection.state == BleConnState.CONNECTED:
            try:
                svc = self.__connection.dev.getServiceByUUID(self.__server.BleServiceUUID)
                self.__char_commu = svc.getCharacteristics(self.__server.BleCommuUUID)[0]
                self.__char_state = svc.getCharacteristics(self.__server.BleStateUUID)[0]
                logging.info('      BLE connected to GATT server %s !\n', self.__server.name)
            except:
                self.__connection.ResetConnection()

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

    def read_characteristic_commu(self):
        try:
            commu_code = self.__char_commu.read()
            return commu_code
        #except bluepy.btle.BTLEDisconnectError:
        except:
            print("ble_read() Device disconnected-- reconnecting")
            self.connect_to_server()
            #continue
            commu_code = self.__char_commu.read()
            return commu_code
    
    def read_characteristic_state(self):
        try:
            state_code = self.__char_state.read()
            return state_code
        #except bluepy.btle.BTLEDisconnectError:
        except:
            print("ble_read() Device disconnected-- reconnecting")
            self.connect_to_server()
            #continue
            state_code = self.__char_state.read()
            return state_code

    def SpinOnce(self):
        if self.__connection.state == BleConnState.CONNECTED:
            try:
                received = self.__char_commu.read()
                logging.info('commu Rx: %s', received)
            except:
                self.__connection.ResetConnection()

        else:
            self.connect_to_server()







if __name__ == '__main__':
    import sys
    logging.basicConfig(level=logging.DEBUG)
    logging.info('@@@@@@@@@@@@@@@@@@@@@@@@@')
    # g_bleClient.scan_arm_house()
    g_bleClient = BleClient('213401')
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
