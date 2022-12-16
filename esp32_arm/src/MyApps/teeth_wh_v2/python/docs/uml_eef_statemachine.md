@startuml
[*] -> idle
state Row {
  state row_idle {
  }
  state row_moving {
  }
}
 

row_idle -> row_moving
row_moving -> row_idle

idle --> picking: row is ready
picking -> check
check --> picking: ir is empty
check --> drop: ir is blocked
drop --> idle

@enduml