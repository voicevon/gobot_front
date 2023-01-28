#include "bolt_nut.h"
#include "MyLibs/basic/logger.h"

void Package::PrintOut(const char * title){
    Logger::Info(title);
    Serial.print("source=");
    Serial.print(net_id);
    Serial.print(" hop= ");
    Serial.print(my_hop);
    Serial.println(" ");
}


bool AdhocHelper::IsSameMacAddr(){
    
}
