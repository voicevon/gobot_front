@startuml
[*] -> idle
state Row {
  state row_idle {
  }
  state row_moving {
  }
  state row_ready {
  }
}

state drop_end <<fork>>
row_moving --> row_ready
row_moving -[#red,bold]-> row_ready

idle -[#red,bold]-> deposit: deposit(row, col)
deposit -[#red,bold]-> row_moving: G1R5C6
row_ready -[#red,bold]-> idle
idle: update(row, col, stock)
idle --> withdraw 
withdraw --> row_moving: G1R5C6

row_ready --> picking: row is ready
picking -> check: .     .
check --> picking: ir is empty <3
check --> drop: ir is blocked
check --> drop: failed (ir is empty >=3)
drop --> drop_end
drop_end --> row_idle
drop_end --> idle: all rows are idle


@enduml
