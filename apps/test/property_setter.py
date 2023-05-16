
class BaseClass():

    x=5

    @property
    @classmethod
    def xx(cls) ->int:
        return cls.x

    @xx.setter
    @classmethod
    def xx(cls, value:int):
        print("Should print setter.")  # Nothing print out.  
        cls.x = value

class MyClass(BaseClass):

    @classmethod
    def Test(cls):
        cls.xx = 'ddd'

class C(object):
    def __init__(self):
        self._x = None

    @property
    def x(self):
        """I'm the 'x' property."""
        print("getter of x called")
        return self._x

    @x.setter
    def x(self, value):
        print("setter of x called")
        self._x = value

    @x.deleter
    def x(self):
        print("deleter of x called")
        del self._x

def main():
    c = C()
    c.x = 'foo'  # setter called
    foo = c.x    # getter called
    del c.x      # deleter called


    MyClass.xx = 222
    print (MyClass.xx)
    MyClass.Test()
    print(MyClass.xx)

main()