# import numpy


class person():
    name = "no name yet"
    def __init__(self, age) -> None:
        self.age = age

    def get_age(self) -> int:
        return self.age

people = [person(99)]


people = []



people.append(person(11))
people.append(person(22))

for p in people:
    print(p.name, p.age)

# p1 = person(11)
# p2 = person(22)
# people = numpy.array( [p1,p2], dtype=person])
# people.append(person(33))
# # people.append(p2)
# people[0].age


