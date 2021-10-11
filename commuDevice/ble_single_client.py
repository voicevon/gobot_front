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

class MyDelegate(btle.DefaultDelegate):
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
                logging.info("BLE CONNECTED to %s" %self.__server_head.BleDeviceName)

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
            print('-----------------Scanning nearby devices------------------')
            print('BleScan() Found: %s  Target:%s' %(name, self.__server_head.BleDeviceName))
            # if name == 'ConInt-Arm-' + self.__gobot_id:
            if name == self.__server_head.BleDeviceName:
                logging.info('---------------------------------------------')
                logging.info('      Discoverd target !')
                self.__server_mac_addr = dev.addr
                self.state = BleConnState.DISCOVERED
                return


class BleSingleClient():
    def __init__(self, _server_head:BleServerHead):
        self.server_head = _server_head
        self.connection = BleConnection(self.server_head)
        self.__char_state = None
        self.__char_commu = None

    def connect_to_server(self):
        self.connection.Connect()
        if self.connection.state == BleConnState.CONNECTED:
            try:
                svc = self.connection.dev.getServiceByUUID(self.server_head.BleServiceUUID)
                self.__char_commu = svc.getCharacteristics(self.server_head.BleCommuUUID)[0]
                self.__char_state = svc.getCharacteristics(self.server_head.BleStateUUID)[0]
                logging.info('      BLE connected to GATT server %s !\n', self.server_head.BleDeviceName)
            except:
                print('ble_single_client.connect_to_server() got exception')
                self.connection.ResetConnection()

    def write_characteristic(self, new_value:str):
        try:
            logging.info('write_characteristic()')
            if self.__char_commu == None:
                return
            self.__char_commu.write(bytes(new_value,encoding='utf8'))
        #except bluepy.btle.BTLEDisconnectError:
        except:
            print('ble_Write() Disconnected --> reconnecting')
            self.connect_to_server()

    def read_characteristic_commu(self) ->str:
        try:
            commu_code = self.__char_commu.read()  # returned type: bytes
            return str(commu_code)
        except:
            print("ble_read() Device disconnected-- reconnecting")
            self.connect_to_server()

    
    def read_characteristic_state(self):
        try:
            state_code = self.__char_state.read()
            return state_code
        #except bluepy.btle.BTLEDisconnectError:
        except:
            print("ble_read() Device disconnected-- reconnecting")
            self.connect_to_server()
            return

    def SpinOnce(self):
        if self.connection.state == BleConnState.CONNECTED:
            try:
                received = self.__char_commu.read()
                # logging.info('commu Rx: %s', received)
            except:
                self.connection.ResetConnection()

        else:
            self.connect_to_server()



if __name__ == '__main__':
    import sys
    logging.basicConfig(level=logging.DEBUG)
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
