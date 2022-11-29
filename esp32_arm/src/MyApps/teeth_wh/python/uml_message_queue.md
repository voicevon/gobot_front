@startuml
[*] -> idle

idle --> deposit: .         .
deposit -> idle: queue is empty
deposit <- deposit

idle --> withdraw
withdraw <- withdraw
withdraw -> idle: queue is empty

@enduml