#include "json_loader.h"
#include "WString.h"
#include <ArduinoJson.h>
#include "MyLibs/basic/logger.h"
#define PIN_IR_CHECKING 32    //   TODO:  This is should not be fixed,  For TWH project only,  right now.


char* Twh4_JsonLoader::Load(){
    const int capacity = JSON_OBJECT_SIZE(3);
    StaticJsonDocument<capacity> doc;
    doc["topic"] = "twh/221109/report";
    if (digitalRead(PIN_IR_CHECKING) == HIGH)
        doc["payload"]["ir_state"] = "empty";
    else
        doc["payload"]["ir_state"] = "blocked";
    doc["payload"]["homed"] = true;
    
    serializeJson(doc, __json_string);
    Logger::Debug("Twh2_JsonLoader::Load()");
    Logger::Print("json_string", __json_string);
    return __json_string;
}