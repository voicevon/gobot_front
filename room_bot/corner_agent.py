from commuDevice.ble_single_client import BleServerHead, BleSingleClient
from commuDevice.gcode_factory import GcodeFactory
import logging
import time

class CornerAgent:
    def __init__(self, server_head:BleServerHead) -> None:
        self.buffer = []
        self.commu_device = BleSingleClient(server_head)

    def append_gcode_string(self, gcode:str) -> None:
        self.buffer.append(gcode)
        logging.info('Appending gcode:' + gcode)
        self.SpinOnce()

    def append_gcode(self, gcode:GcodeFactory) -> None:
        gcode_string = gcode.ToString()
        self.append_gcode_string(gcode_string)

    def wait_robot_be_idle(self) ->None:
        self.append_gcode_string('M280')
        is_idle = False
        while not is_idle:
            self.SpinOnce()
            response = self.commu_device.read_characteristic_state()
            if response == 'IDLE':
                is_idle = True

    def SpinOnce(self) -> None:
        self.commu_device.SpinOnce()
        if self.buffer.__len__() > 0:
            gcode = self.buffer[0]
            self.commu_device.write_characteristic(gcode)
            # double check: server side has received the gcode
            got_ok = False
            while not got_ok:
                response = self.commu_device.read_characteristic_commu()
                if response == None:
                    pass
                elif response == '  > OK':
                    got_ok = True
                    self.buffer.remove(gcode)
                else:
                    logging.info("response of sending gcode from buffer  " + gcode + "   " + response)
                    time.sleep(1)