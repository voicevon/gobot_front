from commuDevice.ble_single_client import BleServerHead


class RoomBot_CornerFactory:
    def __init__(self) -> None:
        pass
        
    def CreateServerHead_XPYP() -> BleServerHead:
        head = BleServerHead()
        head.AxisName ='A'
        head.BleDeviceName = "ConInt-XPYP"
        head.BleServiceUUID = "1fd03a14-1451-11ec-82a8-0242ac130003"
        head.BleCommuUUID = "4b7fbe96-1451-11ec-82a8-0242ac130003"
        head.BleStateUUID = "50711238-1451-11ec-82a8-0242ac130003"
        return head

    def CreateServerHead_XNYP() -> BleServerHead:
        head = BleServerHead()
        head.AxisName='B'
        head.BleDeviceName = "ConInt-XNYP"
        head.BleServiceUUID = "6909288a-1451-11ec-82a8-0242ac130003"
        head.BleCommuUUID = "6e0d5d7e-1451-11ec-82a8-0242ac130003"
        head.BleStateUUID = "72838810-1451-11ec-82a8-0242ac130003"
        return head

    # class XNYN:
    def CreateServerHead_XNYN() -> BleServerHead:
        head = BleServerHead()
        head.AxisName='C'
        head.BleDeviceName = "ConInt-XNYN"
        head.BleServiceUUID = "77d37e60-1451-11ec-82a8-0242ac130003"
        head.BleCommuUUID = "8fdb27b0-1451-11ec-82a8-0242ac130003"
        head.BleStateUUID = "8175103c-1451-11ec-82a8-0242ac130003"
        return head

    # class XPYN:
    def CreateServerHead_XPYN() -> BleServerHead:
        head = BleServerHead()
        head.AxisName='H'
        head.BleDeviceName = "ConInt-XPYN"
        head.BleServiceUUID  = "85f5ab94-1451-11ec-82a8-0242ac130003"
        head.BleCommuUUID = "96a1cd38-1451-11ec-82a8-0242ac130003"
        head.BleStateUUID  = "9c43b29c-1451-11ec-82a8-0242ac130003"
        return head
