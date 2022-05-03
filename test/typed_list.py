
# https://stackoverflow.com/questions/1924469/define-a-list-with-type
class MyClass:
    def __init__(self) -> None:
        pass
    
    def Test(self):
        print("test.....")

obj = MyClass()

# test any type
# anytype_list = []
# anytype_list.append(5)
# anytype_list.append(obj)


spec_type_list = [MyClass]
obj.Test()

# spec_type_list.append(5)
spec_type_list.append(obj)
for oo in spec_type_list:
    print(oo)
    oo.Test()