from abc import ABC, abstractclassmethod


class main:
    def __init__(self, value:int) -> None:
        self.v = value
        
    def add_one(self):
        self.v += 1

    @abstractclassmethod
    def add_more(self):
        ...

    def gogo(self):
        self.add_more(3)

class sub(main):
    def __init__(self) -> None:
        super().__init__(5)

    def add_more(self, vv:int):
        self.v += vv



s = sub()
# s.add_one()
s.gogo()


print(s.v)    