

def foo(x:int)->int:
    print(x)
    # print(x+1)
    return x+1
a = foo(6)
print(a)


# How to force the type to be an int?
foo("\"abc")