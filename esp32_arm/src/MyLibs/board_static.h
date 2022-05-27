#pragma once

#include <HardwareSerial.h>
#include <Arduino.h>

void ReportRam(){
    Serial.println("-----------------  RAM usage report -----------------");
    Serial.print("\t\ttotal heap size = ");
    Serial.println(ESP.getHeapSize()); //total heap size

    Serial.print("\t\tavailable heap = ");
    Serial.println(ESP.getFreeHeap()); //available heap

    Serial.print("\t\tlowest level of free heap since boot = ");
    Serial.println (ESP.getMinFreeHeap()); //lowest level of free heap since boot

    Serial.print("\t\tlargest block of heap that can be allocated at once = ");
    Serial.println(ESP.getMaxAllocHeap()); //largest block of heap that can be allocated at once
}