

class ConfigInternet:
    '''
    TODO  Use ConfigParser with *.INI file?
    https://docs.python.org/3/library/configparser.html
    DISCUSS:  ConfigParser is not strong type, this is the reason we don't like it.
    '''
    class MqttBroker:
        broker_addr = 'voicevon.vicp.io'
        port = 1883
        username = 'von'
        password = 'von1970'
        client = None

    class AppServer:
        ip = 'voicevon.vicp.io'
        port = 50007

    class Wifi:
        ssid = "FuckGFW"
        password = "refuckgfw"



