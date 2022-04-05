
# import numpy
import json
from json import JSONEncoder


class AgvElement:
    # after hardware reseting,  AGV will move forward, 
    # after detecting the first location mark, will go to branch and goto idle.
    def __init__(self, id:int) -> None:
        self.id = id
        self.location = -1   # means unknown.
        self.State = -1   # Charging, moving to load, loading, moving to unload, unloading, idle.
        self.battery_voltate = 12.0

    def on_mqtt_message(self, location_id, battery_voltage, state):
        self.location =  location_id
        self.battery_voltate = battery_voltage
        self.State =  state


class MapElement:
    # def __init__(self) -> None:
    Node_id = 1   # Only on main_road
    MainRoad_IsonLeft = True
    Next_Node_id = 2 # Only on main_road
    SationType = 1   # SHORT_CUT_ONLY, No station
    Next_Node_id_Shortcut = 4 # Only for short cut.  Short cut will always from main_road_node to another main_road_node
    Station_id = 123  # Only on brached_road
    StationType = 1  

class MyJsonEncoder(JSONEncoder):
    '''
    https://pynative.com/make-python-class-json-serializable/
    '''
    def default(self, o):
            return o.__dict__


class MES_Resources:
    '''
    RfCard_id      MainRoad_OnLeft    StationType,   Station_id
    1                   True            SHORT_CUT_ONLY   NULL
    2                   True            LOAD_ONLY        101
    3                   False           SHORT_CUT_ONLY   NULL
    4                   True            LOAD_UNLOAD      203
    5                   True            CHARGE           908
    '''
    def __init__(self) -> None:
        self.all_map_elements=[]
        self.all_agvs=[]

        self.file_name_map = 'config_map.json'
        self.file_name_agvs = 'config_agvs.json'
        self.LoadFromJsonFile()


    def LoadFromJsonFile(self) -> None:
        file = open(self.file_name_map, "r") 
        data = file.read() 
        file.close()
        xx = json.loads(data)
        print(xx) 

        file = open(self.file_name_agvs,"r")
        data = file.read()
        file.close()
        xx = json.loads(data)
        print(xx)

    def GetNode(self, node_id) -> MapElement:
        return self.data

    def AppendMapElement(self, new_node:MapElement) ->None:
        self.all_map_elements.append(new_node)
        JSONData = json.dumps(self.all_map_elements, indent=4, cls=MyJsonEncoder)
        print(JSONData)
        textfile = open(self.file_name_map, "w")
        textfile.write(JSONData)
        textfile.close()

    def AppendAgv(self, new_agv:AgvElement) ->None:
        self.all_agvs.append(new_agv)
        JSONData = json.dumps(self.all_agvs, indent=4, cls=MyJsonEncoder)
        print(JSONData)
        textfile = open(self.file_name_agvs, "w")
        textfile.write(JSONData)
        textfile.close()

if __name__ == '__main__':
    resources = MES_Resources()

    nn = MapElement()
    nn.RfCard_id = 111
    nn.SationType = 222
    nn.Station_id = 333
    nn.MainRoad_IsonLeft = True

    resources.AppendMapElement(nn)
    nn.RfCard_id = 123
    resources.AppendMapElement(nn)

    agv=AgvElement(4444)
    resources.AppendAgv(agv)
    



