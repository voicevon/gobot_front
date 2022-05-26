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