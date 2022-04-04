
from msilib.schema import BBControl
# import numpy
import json


# https://pynative.com/make-python-class-json-serializable/
class AgvMapElement:
    # def __init__(self) -> None:
    RfCard_id = 1
    MainRoad_IsonLeft = True
    SationType = 1   #SHORT_CUT_ONLY
    Station_id = 123

    # def PrintOut(self):
    #     print("RfCard_id=% MainRoad_IsonLeft=%", self.RfCard_id, self.MainRoad_IsonLeft)

class AgvMap:
    '''
    RfCard_id      MainRoad_OnLeft    StationType,   Station_id
    1                   True            SHORT_CUT_ONLY   NULL
    2                   True            LOAD_ONLY        101
    3                   False           SHORT_CUT_ONLY   NULL
    4                   True            LOAD_UNLOAD      203
    5                   True            CHARGE           908
    '''
    def __init__(self) -> None:
        self.file_name = 'tt.json'
        self.all_nodes=[]
        # self.LoadFromJsonFile("all_nodes.json")

    def LoadFromJsonFile(self, file_name) -> None:
        #load from file.
        with open(file_name) as f:
            data = json.load(f)
        for node in data['']:
            print (node['RfCard_id'], node['MainRoad_OnLeft'])

        # self.data = json.loads(file_data)
        self.all_nodes=[]
        print(self.data)

    def GetNode(self, node_id) -> AgvMapElement:
        return self.data

    def AppendNode(self, new_node:AgvMapElement) ->None:
        self.all_nodes.append(new_node)
        # self.all_nodes[0].PrintOut()
        # save to file
        with open(self.file_name, 'w') as f:
            # json.dump(self.all_nodes, f,indent=2)
            json.dump(self.all_nodes, f)


if __name__ == '__main__':
    map = AgvMap()

    nn = AgvMapElement()
    nn.RfCard_id = 111
    nn.SationType = 222
    nn.Station_id = 333
    nn.MainRoad_IsonLeft = True

    map.AppendNode(nn)



