
class myclass:
    def __init__(self, mm:int) -> None:
        self.m= mm
    

def get_list()->list[int]:
    return [1,2,3]

def get_typed_list() -> list[myclass]:
    return [myclass(55),myclass(88)]

a=get_list()
b=get_typed_list()

print(get_list())
for bb in b:
    print(bb.m)