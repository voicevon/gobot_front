# State machine

```   
.     
.      <-----------------------------------------------------------------------------\
,     |                               ^                                              | 
,    F_Paused          S_Paused       |                P_Paused                      |
.     |  ^             |     ^        |[Fast mark]      |    ^                       | [Command Move]                      ^                             
.     |  |             |     |        |                 |    |                       |                           
.   Fast Moving  --> Slow_Moving ------------------>   Parking  ---------> Parked ---^  
.    [slow mark]                       [Park command]   (Super slow)[Sleep command] 
.   from track sensor                                                    
.        

Begin state should be: Unknown  -> Slow_Moving_Paused
```