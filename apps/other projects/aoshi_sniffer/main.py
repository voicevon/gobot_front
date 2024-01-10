import serial
ser = serial.Serial("COM21", 19200)

line = []
previous_id = 0
print("hello")
while True:
     cc = ser.read(1)
     xx = int.from_bytes(cc)
     line.append(xx)
     if len(line) >= 11:
          if (line[0] == 0x03) and (line[1]== 0x09) and (line[2] == 0x24):
               # print(line[5], line[6], line[7], line[8])
               addr = 16777216 * line[5]  +  65536 * line[6] + 256 * line[7] + line[8] 
               # print(addr)
               if addr > 0:
                    if previous_id != addr:
                         print(addr)
                         previous_id = addr
          line.pop(0)


