class Person():
    def __init__(self, age) -> None:
        self.age = age

x = [1,2,3]
x.append('a')
ming = Person(56)
x.append(ming)


print (x[-1].age)