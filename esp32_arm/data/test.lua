print('hello world')
a= 10
b= 20
c = a * b
print(c)

INPUT=1
OUTPUT=3
HIGH = 1
LOW = 0
pinMode(2, OUTPUT)
print("aaaaaaaaaaaaaa")
digitalWrite(2,HIGH)
print("bbbbbbbbbbbbbb")

p = 0
now = millis()
is_on = false

while (true) do     now = millis()     if (now - p) > 1000 then         is_on = not is_on         p= now     end     if is_on then        digitalWrite(2, HIGH)    else        digitalWrite(2, LOW)    end end

--[[
while (true)
do
    now = millis()
    if (now - p) > 1000 then
        is_on = not is_on
        p= now
    end

    if is_on then
        digitalWrite(2, HIGH)
    else
        digitalWrite(2, LOW)
    end
end
]]