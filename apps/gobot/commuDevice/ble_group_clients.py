from ble_single_client import BleSingleClient
import logging


class BleGroupClients:
    def __init__(self, servers):
        self.__servers = servers
        self.__clients = []

    def ConnectAll(self):
        for (server_name, service_uuid, char_commu_uuid, char_state_uuid) in self.__servers:
            new_client = BleSingleClient(server_name)
            new_client.connect_to_server()
            self.__clients.append(new_client)





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