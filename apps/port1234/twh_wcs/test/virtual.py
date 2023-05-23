class Animal():
    def speak(self):
        print("...")

class Cat(Animal):
    def speak(self):
        print("meow")

class Dog(Animal):
    def speak(self):
        print("woof")

my_pets = [Dog(), Cat(), Dog()]

for _pet in my_pets:
     _pet.speak()