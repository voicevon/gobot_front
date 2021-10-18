import numpy as np

class Human:
    def __init__(self) -> None:
        pass
    def ShowName(self):
        print("xxxxxxxxxxxxxxxxx")


#define array, with 500 members
myArray = np.array([Human() for i in range(500)])

#test.
print(myArray[112].ShowName())

