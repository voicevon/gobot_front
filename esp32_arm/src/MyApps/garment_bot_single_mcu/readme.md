# Esp32 Check free memoty
[free_heap](https://techtutorialsx.com/2017/12/17/esp32-arduino-getting-the-free-heap/)

total heap size = 354016
available heap = 327740
lowest level of free heap since boot  322216
largest block of heap that can be allocated at once == 113792


total heap size = 353344
available heap = 323952
lowest level of free heap since boot322216
largest block of heap that can be allocated at once113792

# State machine




```
.     
.     
.                              <---------------------------------------------------------\
,                              |                                                          | 
.   Locating ------------>   Agv Moving ----> Agv Parked ------->align--> Robot_Loading ---^  
.    (Slow)   [Got RFID]        |  (Read Mark RFID)           |                           | 
.                               |                             |-->align--> Robot_Unloading-^
.                             (Fast)                          |                           |
.                             (Slow)                          |-------> Charging ---------^
.                             (SuperSlow)                     |                           |
.                             (Paused)                        |--------> Sleeping --------^
.                                                             |                           |
.                                                             |--------> Charging --------^
```